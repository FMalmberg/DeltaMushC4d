# DeltaMushC4d
The DeltaMush algorithm, originally proposed by Mancewicz et al. [1], smooths arbitrary 
deformation of a polygonal mesh without smoothing the original detail of the model. This is 
useful, e.g., when rigging animated characters. See, e.g., the following demo of the original
implementation by Rythm & Hues:

https://vimeo.com/103854005

This plugin for MAXON Cinema 4d implements the DeltaMush algorithm as a deformer object. (In fact, 
the implemented algorithm is a simplified version of the original one. The modified algorithm is 
easier to implement, but produces equivalent results.)

As with any other C4D deformer, the DeltaMush plugin is used by placing it under the object (mesh) 
to be deformed. In the "reference object" field, you can place another object representing the undeformed 
state of the object. The "Smoothing iterations" field controls how much the deformation between the mesh and
the reference object is smoothed. Higher values give smoother results. If the "reference object" field
is empty, the plugin applies standard mesh smoothing to the deformed object, i.e., the entire mesh is
smoothed and no details are preserved. The plugin is compiled for C4d version 14 under Windows, but should work with
later versions as well.

[1] Mancewicz, Joe, et al. 
"Delta Mush: smoothing deformations while preserving detail." 
Proceedings of the Fourth Symposium on Digital Production. ACM, 2014.
