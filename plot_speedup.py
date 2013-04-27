#!/usr/bin/python
import sys
import matplotlib.pyplot as plt

def plot_speed_up(sort):
    T_serial = -1.0
    p_list = [1, 2, 3, 4, 8]
    time_list = []

    i = 0
    for P in p_list:
        f = open(sort+"_time_P"+str(P)+"_N"+str(N)+".txt", "r")
        execution_time = -1.0
        process = -1
        for line in f:
            p = 0
            if "On process" in line:
                p = int(line.split(":")[1])
            elif "Execution time" in line:
                time = float(line.split(":")[1])
                if (time > execution_time):
                    process = p
                    execution_time = time
                    print "add" + str(execution_time)
                    if (len(time_list)-1 != i):
                        time_list.append(execution_time)
                    else:
                        time_list[i] = execution_time
                    if P == 1:
                        T_serial = execution_time
        i+=1
        print "---NUMBER OF PROCESSES "+str(P)+" ---"
        print "\tExecution time "+str(execution_time)
        print "\tReached for processors " + str(process)
        print "\tSpeed-up " + str(T_serial/execution_time)
        print "\tEfficiency " + str(T_serial/(execution_time*P))

    print time_list
    speed_up_list = [ time_list[0]/t for t in time_list]
    print speed_up_list
    plt.clf()
    plt.plot(p_list, speed_up_list)
    plt.savefig(sort+"_N"+str(N)+".png")

if __name__ == "__main__":
    N = int(sys.argv[1])
    plot_speed_up("odd_even_sort")
    plot_speed_up("merge_sort")
    plot_speed_up("bucket_sort")


