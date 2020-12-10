import simexpal
import yaml
import matplotlib.pyplot as plt
import numpy as np

ds = ["5b", "5c"]
for d in ds:
    cfg = simexpal.config_for_dir(d)

    results = []
    for run in cfg.collect_successful_results():
        with run.open_output_file() as f:
            yml = yaml.load(f, Loader=yaml.SafeLoader)
        results.append(yml)

    results = sorted(results, key=lambda r: (r['num_threads']))


    def getValues(key, algo):
        return [result[key] for result in results if result['num_threads'] == algo]

    algos = [1, 2, 4, 8, 16, 32, 64]
    keys = ['time']

    for key in keys:
        plt.clf()
        for algo in algos:
            # plotting the lines
            plt.plot(np.mean(getValues('num_threads', algo)), np.mean(getValues(key, algo)), "o", label=algo)

        # set labels
        plt.xlabel('Threads')
        plt.ylabel(key)
        # Set title
        plt.title('Threads vs. ' + key)
        # show a legend on the plot
        plt.legend()
        # Display a figure.
        # plt.show()

        plt.savefig(d+'-' + key + '.png')
        plt.savefig(d+'-' + key + '.pdf')