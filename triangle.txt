# Assumes a right-angled triangle with M rows and N columns, scaling the number of stars per row.
print('\n'.join('*'*((i+1)*4//3) for i in range(3)))

# Same assumptions as above, takes M and N as arguments.
def triangle(M,N):print('\n'.join('*'*((i+1)*N//M)for i in range(M)))