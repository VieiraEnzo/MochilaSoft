import numpy as np


class Instance:
    def __init__(self, file):
        # uncoment to run tuning:
        self.filename = file
        path = file
        # path = f"instances/{file['instance_type']}/{file['filename']}"
        # self.filename = file['filename']

        f = open(path, "r")

        self.num_items, self.num_forfeits_pairs, self.budget = map(
            int, f.readline().split(" ")
        )

        f.close()

        # items definition
        items = []
        for i in range(self.num_items):
            items.append(i)

        self.items = np.array(items)

        line_counter = 1

        self.profits = np.loadtxt(
            path, dtype=int, delimiter=" ", skiprows=line_counter, max_rows=1
        )
        line_counter += 1

        self.weights = np.loadtxt(
            path, dtype=int, delimiter=" ", skiprows=line_counter, max_rows=1
        )
        line_counter += 1

        max = 2 * self.num_forfeits_pairs

        self.forfeit_cost_and_forfeits_pairs = np.loadtxt(
            path,
            dtype=int,
            delimiter=" ",
            skiprows=line_counter,
            max_rows=max,
            usecols=(0, 1),
        ).tolist()

        self.forfeits_costs = [
            v for i, v in enumerate(self.forfeit_cost_and_forfeits_pairs) if i % 2 == 0
        ]

        self.forfeits_pairs = [
            v for i, v in enumerate(self.forfeit_cost_and_forfeits_pairs) if i % 2 != 0
        ]

        # remove the first element of the sublist
        for i in range(len(self.forfeits_costs)):
            self.forfeits_costs[i].pop(0)

        # transform list of lists into a single list - flatten
        self.forfeits_costs = [
            item for sublist in self.forfeits_costs for item in sublist
        ]

        self.forfeits_costs = np.array(self.forfeits_costs)
        self.forfeits_pairs_array = np.array(self.forfeits_pairs)

        # set conversion for efficiency - search O(1)
        self.forfeits_pairs_set = {tuple(x) for x in self.forfeits_pairs_array}

    def forfeit_cost_pairs_mx(self):
        mD = np.zeros((int(len(self.items)), int(len(self.items))), dtype=int)

        for index, pair in enumerate(self.forfeits_pairs):
            mD[int(pair[0]), int(pair[1])] = self.forfeits_costs[index]
            mD[int(pair[1]), int(pair[0])] = self.forfeits_costs[
                index
            ]  # comment to take the triangular superior mD

            # mD = np.triu(mD, k=0) # take the triangular superior
        return mD

    def debug_read_instance(self):

        print(f"items:{self.items}")
        print(f"num forfeits_pairs:{self.num_forfeits_pairs}")
        print(f"knapsack size:{self.budget}")
        print(f"profits:{self.profits}")
        print(f"weights:{self.weights}")
        print(f"forfeits_pairs:{self.forfeits_pairs}")
        print(f"forfeits_costs:{self.forfeits_costs}")
