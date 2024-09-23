import numpy as np
import math

class Solution:
    def __init__(self, instance, solution_itens=None) -> None:
        self.instance = instance
        if solution_itens:
            self.items = np.array(solution_itens) # solution items
        else:
            self.items = np.array([], dtype=int) # solution items
            
        
    def get_cost(self, mD) -> int:
        cost = 0
        
        for item in self.items:
            cost += self.instance.profits[item]
  
        for i in self.items:
            for j in self.items:
                if i < j: 
                    cost = cost - mD[i][j]

        return cost

    def add_item(self, item) -> None:
        self.items = np.append(self.items, item)

    def remove_item(self, item)-> None:
        self.items = np.delete(self.items, np.where(self.items == item))

    def remove_last_choices(self, removal_percentage) -> None:
        items_removed = int(len(self.items) * removal_percentage)
        self.items = self.items[:-items_removed]

    def remove_oldest_choices(self)-> list:
        self.items = self.items[1:]

    def remove_oldest_choices_adaptative(self, p)-> list:
        
        num_items_to_extract = math.ceil(len(self.items) * p)
        adaptative_list = self.items[:num_items_to_extract]

        random_element = np.random.choice(adaptative_list)
        self.items = np.delete(self.items, np.where(self.items == random_element))
        
    def solution_items(self):
         return self.items

    def get_profit(self):
        pass

    def get_residual_budget(self):
        bres  = self.instance.budget
        for item in self.items:
            bres = bres - self.instance.weights[item]    
        
        return bres
    

    def get_forfeit_cost_item_in_solution(self, item:int, mD:np.ndarray, forfeits_pairs_set, current=True):
        forfeit_cost_item = 0
        forfeit_cost_item = self.instance.profits[item]

        if current:
            for sol in self.items:
                if (sol, item) in forfeits_pairs_set:
                    forfeit_cost_item = forfeit_cost_item - mD[sol][item]
        else:       
            for sol in self.items:
                    if (sol, item) in forfeits_pairs_set:
                        forfeit_cost_item = forfeit_cost_item - mD[sol][item]
        
        return forfeit_cost_item

    def get_solution_forfeit_cost(self):
        pass

    def is_feasible_i_j(self, out_item, in_item):

        #swap test
        self.add_item(in_item)
        self.remove_item(out_item)
    
        knapsack_weight = 0
        for i in self.items:
            knapsack_weight += self.instance.weights[i]

        total_capacity = self.instance.budget
        self.add_item(out_item)
        self.remove_item(in_item)
        if knapsack_weight <= total_capacity :
            return True
        
        return False
    
    def is_feasible(self):
    
        knapsack_weight = 0
        for i in self.items:
            knapsack_weight += self.instance.weights[i]

        total_capacity = self.instance.budget
        if knapsack_weight <= total_capacity :
            return True
        
        return False
    
    def get_paid_forfeits(self, forfeits_pairs_set, mD):
        num_paid_forfeits = 0

        list_forfeits = []
        
        for i in self.items:
            for j in self.items:
                if (i, j) in forfeits_pairs_set:
                    list_forfeits.append((i,j))
                    num_paid_forfeits+=1

        if len(list_forfeits) != len(set(list_forfeits)):
            print("repeated elements")

        # preciso arrumar os pares 
        # for i in self.items:
        #     for j in self.items:
        #         sum_forfeit = 0
        #         if (i, j) in forfeits_pairs_set and (j, i) in forfeits_pairs_set:
        #             print(f"{(i,j)} -> {mD[i][j]}")
        #             print(f"{(j,i)} -> {mD[j][i]}")
        #             sum_forfeit +=  (mD[i][j] + mD[j][i])
        #             print(sum_forfeit)


        # print(list_forfeits)

        return num_paid_forfeits


    #metodos
    #profit da solucao
    #forfeits da solucao --> calcular uma vez e colocar numa variavel (soma dos forfeits)
    # calcular o movimento de item em uma funçao

    # lista de forfeits de items da solução (forfeit de um com todo mundo) -->> somar as linhas
    # do item1 com todo mundo, item2 com todo mundo, item3 com todo mundo...

    # escrever o que calcula talvez calculamos toda hora. 
