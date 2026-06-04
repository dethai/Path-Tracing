import matplotlib.pyplot as plt

threads = [1, 2, 3, 4, 8, 16]
times = [253.368, 163.835, 141.649, 126.835, 113.711, 144.447]

speedup = [times[0] / t for t in times]

plt.figure(figsize=(10,4))

# Execution time
plt.subplot(1,2,1)
plt.plot(threads, times, marker='o')
plt.xlabel('Number of Threads')
plt.ylabel('Execution Time (s)')
plt.title('OpenMP Execution Time')
plt.grid(True)

# Speedup
plt.subplot(1,2,2)
plt.plot(threads, speedup, marker='o', label='Measured')
plt.plot(threads, threads, '--', label='Ideal')
plt.xlabel('Number of Threads')
plt.ylabel('Speedup')
plt.title('OpenMP Speedup')
plt.grid(True)
plt.legend()

plt.tight_layout()
plt.show()