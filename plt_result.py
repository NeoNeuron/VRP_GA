import numpy as np
import matplotlib.pyplot as plt

f = open("optimal_strategy.csv")
dat = []
new_depot = 0
for line in f:
    elements = line.split('\t')
    elements = [int(x) for x in elements]
    if elements[0] != 0:
        new_depot = elements[0]
    dat.append(elements)
f.close()

initial_depot = np.array([0, 50, 0])
customer_list = np.loadtxt("vrp_dat.csv", delimiter = ',')
customer_list = np.vstack([initial_depot, customer_list]);

plt.figure(figsize=(16,16), dpi = 80)
# plot customers;
plt.scatter(initial_depot[0], initial_depot[1], marker = '*', s = 200, c = 'r', label = 'Original Depot')
for i in range(1, len(customer_list)):
    if i == new_depot:
        plt.scatter(customer_list[i,0], customer_list[i,1], s = customer_list[i,2]*4, marker = 's', c = 'g', label = 'New Depot')
    else:
        plt.scatter(customer_list[i,0], customer_list[i,1], s = customer_list[i,2]*4, c = 'k')

# draw lines;
for i in range(len(dat)):
    plt.plot(customer_list[dat[i], 0], customer_list[dat[i], 1], label = 'Route ' + str(i + 1))
plt.legend()
plt.savefig('optimal_strategy.png')
plt.close()

tendency = np.loadtxt("optimal_least_path.csv")
plt.figure(figsize=(16,9), dpi = 80)
plt.plot(tendency)
plt.xlabel("Number of generation", fontsize = 20)
plt.ylabel("Minimum of total distance", fontsize = 20)
plt.grid()
plt.savefig('optimal_least_path.png')
plt.close()
