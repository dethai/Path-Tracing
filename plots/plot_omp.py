import matplotlib.pyplot as plt

threads = [1, 2, 3, 4, 8, 16]
times = [253.368, 163.835, 141.649, 126.835, 113.711, 144.447]

speedup = [times[0] / t for t in times]
efficiency = [s / p for s, p in zip(speedup, threads)]

plt.figure(figsize=(15,4))

# Execution time
plt.subplot(1,3,1)
plt.plot(threads, times, marker='o')
plt.xlabel('Number of Threads')
plt.ylabel('Execution Time (s)')
plt.title('OpenMP Execution Time')
plt.grid(True)

# Speedup
plt.subplot(1,3,2)
plt.plot(threads, speedup, marker='o', label='Measured')
plt.plot(threads, threads, '--', label='Ideal')
plt.xlabel('Number of Threads')
plt.ylabel('Speedup')
plt.title('OpenMP Speedup')
plt.grid(True)
plt.legend()

# Efficiency
plt.subplot(1,3,3)
plt.plot(threads, efficiency, marker='o')
plt.xlabel('Number of Threads')
plt.ylabel('Efficiency')
plt.title('OpenMP Efficiency')
plt.grid(True)

plt.tight_layout()
plt.show()