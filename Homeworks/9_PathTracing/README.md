# 9. Ray Tracing

> Rainzor

## Content

- Implement path tracing algorithm.
- Importance sampling for environment map lighting.
- Set up scene (code, json) and render it

The requirements and methods based on the contents of two folders: `(1) documents` and `(2) project`.

### (1) Documentation `documents` [->](documents/) 

Requirements and some auxiliary materials for this assignment.

### (2) Source code `project ` [->](project/)

Basic code framework for this project (Complete).

## Result

<center>
    <img style = "
        border-radius: 0.3125em;
        box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);" 
        src = ".\images\img-path-tracing\rst_dir.png" 
        width = "50%">
    <br>
    <div style = "
        color: orange;
        border-bottom: 1px solid #d9d9d9;
        display: inline-block;
        color: #999;
        padding: 2px;">
        Figure 1: Only direct light
    </div>
    <p> </p>
</center>

<center>
    <img style = "
        border-radius: 0.3125em;
        box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);" 
        src = ".\images\img-path-tracing\rst_full.png" 
        width = "50%">
    <br>
    <div style = "
        color: orange;
        border-bottom: 1px solid #d9d9d9;
        display: inline-block;
        color: #999;
        padding: 2px;">
        Figure 2: Full light
    </div>
    <p> </p>
</center>

<center>
    <img style = "
        border-radius: 0.3125em;
        box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);" 
        src = ".\images\img-path-tracing\rst_full_impSamp.png" 
        width = "50%">
    <br>
    <div style = "
        color: orange;
        border-bottom: 1px solid #d9d9d9;
        display: inline-block;
        color: #999;
        padding: 2px;">
        Figure 3: Full light by important sampling
    </div>
    <p> </p>
</center>

<center>
    <img style = "
        border-radius: 0.3125em;
        box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);" 
        src = ".\images\img-path-tracing\rst_high_spp.png" 
        width = "50%">
    <br>
    <div style = "
        color: orange;
        border-bottom: 1px solid #d9d9d9;
        display: inline-block;
        color: #999;
        padding: 2px;">
        Figure 4: Full light in high spp
    </div>
    <p> </p>
</center>
