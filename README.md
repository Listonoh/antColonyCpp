# Ant Colony C++
ant colony optimization in c++. On traveling salesman problem.

## Usage
ant.exe "jemnoSouboru" --alfa 1 --beta 0.9 --rho 0.01 --Q 2 --max 100
kde alfa, beta, rho, Q a max jsou nepovine argumenty. Které určují jak se bude funkce popsaná níže chovat (max je maximální počet generací). 
vrátí hodnotu cesty a cestu samotnou na standartní výstup ve formátu:
``` python
final size: 17
final value: 43
1 2 7 10 13 12 8 3 7 11 7 4 1 5 9 5 1
```

## pseudocode (for my solution)
``` python
 # mp - is loaded map with initialized edges, vertexes and pheromones
 # n - number of ants
solution AntColonyTSP(mp, n): 
    while(! Optimal):
		for each ant:
			generateSolutions()
			compareIfBest()
		pheromoneUpdateFromTheBest()
	return BestSolution
```

## What it is about (source en wiki) 
In computer science and operations research, the ant colony optimization algorithm (ACO) is a probabilistic technique for solving computational problems which can be reduced to finding good paths through graphs.
 Artificial Ants stand for multi-agent methods inspired by the behavior of real ants. The pheromone-based communication of biological ants is often the predominant paradigm used.
 Combinations of Artificial Ants and local search algorithms have become a method of choice for numerous optimization tasks involving some sort of graph, e.g., vehicle routing and internet routing.

### How it works
	
In the natural world, ants of some species (initially) wander randomly, and upon finding food return to their colony while laying down pheromone trails.
If other ants find such a path, they are likely not to keep travelling at random, but instead to follow the trail, returning and reinforcing it if they eventually find food (see Ant communication). 
Over time, however, the pheromone trail starts to evaporate, thus reducing its attractive strength. The more time it takes for an ant to travel down the path and back again, the more time the pheromones have to evaporate.
 A short path, by comparison, gets marched over more frequently, and thus the pheromone density becomes higher on shorter paths than longer ones.
 Pheromone evaporation also has the advantage of avoiding the convergence to a locally optimal solution.
 If there were no evaporation at all, the paths chosen by the first ants would tend to be excessively attractive to the following ones.
 In that case, the exploration of the solution space would be constrained. The influence of pheromone evaporation in real ant systems is unclear, but it is very important in artificial systems.

For calculating pheromones:
![p_{xy}^{k}={\frac {(\tau _{xy}^{\alpha })(\eta _{xy}^{\beta })}{\sum _{z\in \mathrm {allowed} _{x}}(\tau _{xz}^{\alpha })(\eta _{xz}^{\beta })}}](https://wikimedia.org/api/rest_v1/media/math/render/svg/a55af3e02c1e319b5b8be2c626e6cf851e8078f9)

For updating pheromones :
$$
\tau_{xy} = (1 - \rho) + \Delta\tau_{xy}^k
$$

![\Delta \tau _{xy}^{k}={\begin{cases}Q/L_{k}&{\mbox{if ant }}k{\mbox{ uses curve }}xy{\mbox{ in its tour}}\\0&{\mbox{otherwise}}\end{cases}}](https://wikimedia.org/api/rest_v1/media/math/render/svg/da75f512c94f2b2737112bebbf97539f5f6928c0)

## Resources
faceobook.txt data are from: http://snap.stanford.edu/data/ego-Facebook.html

> Written with [StackEdit](https://stackedit.io/).
