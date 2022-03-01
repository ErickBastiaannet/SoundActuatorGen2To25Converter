# Sound actuator gen2 to 2.5 converter
 
The soundactuator is a vehicle audio system used to simulate engine noise inside and outside a car. This module reads and generates can signales to use a Generation 2.5 module in
a Generation 2 car. The software currently supports the Audi A6 4G Facelift. Original modules 4G0 907 159 XX and 4G0 907 160 XX are replaced with 4K0 907 159 XX and 4G0 907 160 XX, with this converter in place the right can signals are generated on the can-bus system. Before you can use any module you need to edit the VIN- and Enginecode mask. How to do that is described on the next github link through JilleB
https://github.com/ErickBastiaannet/mqb-soundaktor

## Requirements
- Arduino Mini Pro
- MCP2515 Can Controller


## Tested Cars
- Audi A6 4G Facelift
