# Inverse distance-weighted interpolation methods[^IDW] 

$$
\pmb{f}(\pmb{p})=\sum w_i(\pmb{p})\pmb{f}_i(\pmb{p})
$$

其中

$$
w_i(\pmb{p})=\frac{\sigma_i(\pmb{p})}{\sum \sigma_j(\pmb{p})}
$$

$$
\sigma_i(\pmb{p})=\frac{1}{\|\pmb{p}-\pmb{p_i}\|^u}
$$

显然 $0\le w_i(\pmb{p})\le 1$，且 $\sum w_i(\pmb{p})=1$ 

## 参考文献

[^IDW]: Ruprecht D, Muller H. [**Image warping with scattered data interpolation**](http://citeseer.ist.psu.edu/426836.html)[J]. IEEE Computer Graphics and Applications, 1995, 15(2): 37-43.

