import numpy as np


def execute_greedyforfeits(problem_instance, mD, solution):
    
    items = problem_instance.items
    weights = problem_instance.weights
    profits = problem_instance.profits
    forfeits_pairs_set = problem_instance.forfeits_pairs_set
    S = set() 
    bres = problem_instance.budget  # residual budget
    items_set = set(items)

    remaining_items = items_set.difference(S)  # inicialmente com todos os elementos

    while remaining_items:
        Xiter = []  # Contém items que ainda podem ser adicionados na solução
        ratios = dict()

        for i in items:
            if weights[i] <= bres and i not in S:
                Xiter.append(i)

        if not Xiter:
            return 
        
        for i in Xiter:
            profit = profits[i]
            for j in S:
                if (i, j) in forfeits_pairs_set or (j, i) in forfeits_pairs_set:
                    profit = profit - mD[i][j]
                    
            ratios.update({i: profit/weights[i]})
            

        if not ratios:
            return 
        
        best_item = max(ratios, key=ratios.get)
            
        if ratios[best_item] < 0:
            return 

        S.add(best_item)
        solution.items = np.append(solution.items, best_item)

        # if not solution.test_feasibility():
        #     print(f"Solution not Feasible: {solution.items}")

        # print(solution.get_paid_forfeits(forfeits_pairs_set))
        # cost_inter = solution.get_cost(mD, forfeits_pairs_set)
        # print(cost_inter)

        bres = bres - weights[best_item]
        remaining_items = items_set.difference(S)
        