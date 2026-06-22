import matplotlib.pyplot as plt

processes = [1, 2, 4, 8, 16]
times = [5359.9, 2809.2, 1728.99, 865.471, 470.071]

speedup = [times[0] / t for t in times]
efficiency = [s / p for s, p in zip(speedup, processes)]

plt.figure(figsize=(15,4))

# Execution time
plt.subplot(1,3,1)
plt.plot(processes, times, marker='o')
plt.xlabel('Number of Processes')
plt.ylabel('Execution Time (s)')
plt.title('MPI Execution Time')
plt.grid(True)

# Speedup
plt.subplot(1,3,2)
plt.plot(processes, speedup, marker='o', label='Measured')
plt.plot(processes, processes, '--', label='Ideal')
plt.xlabel('Number of Processes')
plt.ylabel('Speedup')
plt.title('MPI Speedup')
plt.grid(True)
plt.legend()

# Efficiency
plt.subplot(1,3,3)
plt.plot(processes, efficiency, marker='o')
plt.xlabel('Number of Processes')
plt.ylabel('Efficiency')
plt.title('MPI Efficiency')
plt.grid(True)

plt.tight_layout()
plt.show()