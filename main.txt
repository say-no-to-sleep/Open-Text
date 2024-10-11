from selenium import webdriver
from selenium.webdriver.chrome.service import Service
from selenium.webdriver.common.by import By
from selenium.webdriver.chrome.options import Options
from webdriver_manager.chrome import ChromeDriverManager
from bs4 import BeautifulSoup
import json
import time

# URL of the product listing page
URL = "https://www.microfocus.com/en-us/products?trial=true"

# Set up Chrome options
chrome_options = Options()
chrome_options.add_argument("--headless=new")  # Run headless browser to avoid opening a UI window
chrome_options.add_argument("--disable-gpu")  # Disables GPU hardware acceleration
chrome_options.add_argument("--no-sandbox")
chrome_options.add_argument("--disable-dev-shm-usage")
chrome_options.add_argument("--remote-debugging-port=9222")  # Opens a port for remote debugging


# Set up the WebDriver
service = Service(ChromeDriverManager().install())
driver = webdriver.Chrome(service=service, options=chrome_options)

# Open the webpage
# This initiates the browser to load the given URL
driver.get(URL)

# Allow some time for JavaScript to load
# The JavaScript content takes some time to fully load, hence the delay
time.sleep(5)

# Extract the specific section with id 'section5'
try:
    # Locate the section with id 'section5'
    section5 = driver.find_element(By.ID, "section5")
    # Extract the HTML content of the section
    section5_html = section5.get_attribute('outerHTML')

    # Parse the HTML content with BeautifulSoup
    soup = BeautifulSoup(section5_html, 'html.parser')
    products = []  # List to store product information

    # Find all product cards in the section
    product_cards = soup.find_all('div', class_='uk-card uk-card-hover uk-card-default uk-overflow-hidden uk-margin-large-bottom uk-position-relative')
    for card in product_cards:
        # Extract product name
        # Find the product name within the card
        product_name_tag = card.find('h3', class_='uk-card-title')
        product_name = product_name_tag.get_text(strip=True) if product_name_tag else ""

        # Extract starting letter
        # Get the first letter of the product name
        starting_letter = product_name[0].upper() if product_name else ""

        # Extract description
        # Find the product description within the card
        description_tag = card.find('div', class_='description')
        description = description_tag.get_text(strip=True) if description_tag else "No description available"

        # Extract Free Trial / Demo Request URL
        # Find the link for a free trial or demo request
        free_trial_link_tag = card.find('a', class_='uk-button', string=lambda x: x and ('free trial' in x.lower() or 'request demo' in x.lower()))
        free_trial_link = free_trial_link_tag['href'] if free_trial_link_tag and 'href' in free_trial_link_tag.attrs else None

        # Extract Support Link URL from footer
        # Locate the footer section of the card to find support and community links
        footer = card.find('div', class_='footer')
        if footer:
            # Find the support link within the footer
            support_link_tag = footer.find('a', href=True, string=lambda x: x and 'Support' in x)
            support_link = support_link_tag['href'] if support_link_tag else None

            # Extract Community Link URL from footer
            community_link_tag = footer.find('a', href=True, string=lambda x: x and 'Community' in x)
            community_link = community_link_tag['href'] if community_link_tag else None
        else:
            # If the footer is not found, set the links to None
            support_link = None
            community_link = None

        # Append product info to list
        # Collect all the extracted information into a dictionary and add it to the products list
        products.append({
            'Product Name': product_name,
            'Starting Letter': starting_letter,
            'Description': description,
            'Free Trial / Demo Request URL': free_trial_link,
            'Support Link URL': support_link,
            'Community Link URL': community_link
        })

    # Write the product list to JSON file
    # Save the extracted product data into a JSON file
    with open('products.json', 'w', encoding='utf-8') as json_file:
        json.dump(products, json_file, indent=4)
    print("Products extracted successfully and saved to 'products.json'")

except Exception as e:
    # Print any error that occurs during extraction
    print(f"Error extracting section5: {e}")

# Close the browser
driver.quit()