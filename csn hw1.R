---
title: "R Notebook"
output: html_notebook
---

```{r}
library(igraph)
```


```{r}
n = 21
apl = rep(NA, n)
cc = rep(NA, n)
probs = seq(from=0, to = 1, by = 1/20)

for(i in 1:probs){
  temp_apl = rep(NA,50)
  temp_cc = rep(NA,50)
  for (j in 1:50){ 
    watts.strogatz.game(dim = 1, size = 1000, nei = 4, p = i)
  }

```

```{r}
hist(sample(seq(from=0, to = 1, by = 1/20), 100000, replace = TRUE), probability = rep(1/20, 20), breaks = 20, freq = TRUE)

```

```{r}
a = seq(from=0, to=1, by= 1/20)
probs = rep(a, each=100)
norm_graph = watts.strogatz.game(1,100,4,0)

cc = function(g) {
  transitivity(g)/transitivity(norm_graph)
  }

apl = function(g){
  average.path.length(g)/average.path.length(norm_graph)
}

create_ws = function(p){
  watts.strogatz.game(1,100,4,p)
}

graphs = lapply(probs, create_ws)

cluster_coeffs = sapply(graphs, cc)
apl_coeffs = sapply(graphs, apl)
```

```{r}

cluster_coeffs
```

```{r}
plot(cluster_coeffs, type = "o")
```

```{r}
plot(apl_coeffs, type = "o")
```

```{r}
#colMeans(cluster_coeffs, , 100)
```

