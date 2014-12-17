//
//  WaveformComponent.h
//  NebuloMod
//
//  Created by Ryan Foo on 12/16/14.
//
//

#ifndef __NebuloMod__WaveformComponent__
#define __NebuloMod__WaveformComponent__

#include "CurvePoint.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include "dRowAudio_MathsUtilities.h"
#include "dRowAudio_BezierCurve.h"
#include "LFOWaveformTable.h"


/* WARNING: IF YOU CHANGE THE SIZE OF THIS TABLE YOU MUST ALSO CHANGE THE SIZE OF
 THE tableBuffer IN LFOWaveformTable!!!!!!!! */


static float defaults[1024] = {0.006, 0.012, 0.018, 0.025, 0.031, 0.037, 0.043, 0.049, 0.055, 0.061,
    0.067, 0.073, 0.080, 0.086, 0.092, 0.098, 0.104, 0.110, 0.116, 0.122, 0.128, 0.134, 0.140, 0.147,
    0.153, 0.159, 0.165, 0.171, 0.177, 0.183, 0.189, 0.195, 0.201, 0.207, 0.213, 0.219, 0.225, 0.231,
    0.237, 0.243, 0.249, 0.254, 0.260, 0.266, 0.272, 0.278, 0.284, 0.290, 0.296, 0.302, 0.307, 0.313,
    0.319, 0.325, 0.331, 0.336, 0.342, 0.348, 0.354, 0.359, 0.365, 0.371, 0.376, 0.382, 0.388, 0.393,
    0.399, 0.405, 0.410, 0.416, 0.421, 0.427, 0.432, 0.438, 0.443, 0.449, 0.454, 0.460, 0.465, 0.471,
    0.476, 0.482, 0.487, 0.492, 0.498, 0.503, 0.508, 0.513, 0.519, 0.524, 0.529, 0.534, 0.539, 0.545,
    0.550, 0.555, 0.560, 0.565, 0.570, 0.575, 0.580, 0.585, 0.590, 0.595, 0.600, 0.605, 0.610, 0.614,
    0.619, 0.624, 0.629, 0.634, 0.638, 0.643, 0.648, 0.652, 0.657, 0.662, 0.666, 0.671, 0.675, 0.680,
    0.684, 0.689, 0.693, 0.698, 0.702, 0.706, 0.711, 0.715, 0.719, 0.723, 0.728, 0.732, 0.736, 0.740,
    0.744, 0.748, 0.752, 0.756, 0.760, 0.764, 0.768, 0.772, 0.776, 0.780, 0.784, 0.787, 0.791, 0.795,
    0.799, 0.802, 0.806, 0.810, 0.813, 0.817, 0.820, 0.824, 0.827, 0.831, 0.834, 0.837, 0.841, 0.844,
    0.847, 0.851, 0.854, 0.857, 0.860, 0.863, 0.866, 0.869, 0.872, 0.875, 0.878, 0.881, 0.884, 0.887,
    0.890, 0.892, 0.895, 0.898, 0.901, 0.903, 0.906, 0.908, 0.911, 0.913, 0.916, 0.918, 0.921, 0.923,
    0.926, 0.928, 0.930, 0.932, 0.935, 0.937, 0.939, 0.941, 0.943, 0.945, 0.947, 0.949, 0.951, 0.953,
    0.955, 0.956, 0.958, 0.960, 0.962, 0.963, 0.965, 0.966, 0.968, 0.970, 0.971, 0.972, 0.974, 0.975,
    0.977, 0.978, 0.979, 0.980, 0.982, 0.983, 0.984, 0.985, 0.986, 0.987, 0.988, 0.989, 0.990, 0.991,
    0.991, 0.992, 0.993, 0.994, 0.994, 0.995, 0.996, 0.996, 0.997, 0.997, 0.998, 0.998, 0.998, 0.999,
    0.999, 0.999, 0.999, 1.000, 1.000, 1.000, 1.000, 1.000, 1.000, 1.000, 1.000, 1.000, 1.000, 0.999,
    0.999, 0.999, 0.999, 0.998, 0.998, 0.997, 0.997, 0.997, 0.996, 0.995, 0.995, 0.994, 0.994, 0.993,
    0.992, 0.991, 0.990, 0.990, 0.989, 0.988, 0.987, 0.986, 0.985, 0.984, 0.982, 0.981, 0.980, 0.979,
    0.978, 0.976, 0.975, 0.974, 0.972, 0.971, 0.969, 0.968, 0.966, 0.965, 0.963, 0.961, 0.960, 0.958,
    0.956, 0.954, 0.952, 0.950, 0.949, 0.947, 0.945, 0.943, 0.940, 0.938, 0.936, 0.934, 0.932, 0.930,
    0.927, 0.925, 0.923, 0.920, 0.918, 0.915, 0.913, 0.910, 0.908, 0.905, 0.903, 0.900, 0.897, 0.895,
    0.892, 0.889, 0.886, 0.883, 0.881, 0.878, 0.875, 0.872, 0.869, 0.866, 0.863, 0.859, 0.856, 0.853,
    0.850, 0.847, 0.843, 0.840, 0.837, 0.833, 0.830, 0.826, 0.823, 0.820, 0.816, 0.812, 0.809, 0.805,
    0.802, 0.798, 0.794, 0.790, 0.787, 0.783, 0.779, 0.775, 0.771, 0.767, 0.763, 0.760, 0.756, 0.751,
    0.747, 0.743, 0.739, 0.735, 0.731, 0.727, 0.723, 0.718, 0.714, 0.710, 0.705, 0.701, 0.697, 0.692,
    0.688, 0.683, 0.679, 0.674, 0.670, 0.665, 0.661, 0.656, 0.651, 0.647, 0.642, 0.637, 0.633, 0.628,
    0.623, 0.618, 0.613, 0.609, 0.604, 0.599, 0.594, 0.589, 0.584, 0.579, 0.574, 0.569, 0.564, 0.559,
    0.554, 0.549, 0.544, 0.538, 0.533, 0.528, 0.523, 0.518, 0.512, 0.507, 0.502, 0.496, 0.491, 0.486,
    0.480, 0.475, 0.470, 0.464, 0.459, 0.453, 0.448, 0.442, 0.437, 0.431, 0.426, 0.420, 0.415, 0.409,
    0.403, 0.398, 0.392, 0.387, 0.381, 0.375, 0.370, 0.364, 0.358, 0.352, 0.347, 0.341, 0.335, 0.329,
    0.324, 0.318, 0.312, 0.306, 0.300, 0.294, 0.289, 0.283, 0.277, 0.271, 0.265, 0.259, 0.253, 0.247,
    0.241, 0.235, 0.229, 0.224, 0.218, 0.212, 0.206, 0.200, 0.194, 0.188, 0.182, 0.175, 0.169, 0.163,
    0.157, 0.151, 0.145, 0.139, 0.133, 0.127, 0.121, 0.115, 0.109, 0.103, 0.097, 0.091, 0.084, 0.078,
    0.072, 0.066, 0.060, 0.054, 0.048, 0.042, 0.035, 0.029, 0.023, 0.017, 0.011, 0.005, -0.001, -0.007,
    -0.014, -0.020, -0.026, -0.032, -0.038, -0.044, -0.050, -0.056, -0.062, -0.069, -0.075, -0.081,
    -0.087, -0.093, -0.099, -0.105, -0.111, -0.117, -0.123, -0.130, -0.136, -0.142, -0.148, -0.154,
    -0.160, -0.166, -0.172, -0.178, -0.184, -0.190, -0.196, -0.202, -0.208, -0.214, -0.220, -0.226,
    -0.232, -0.238, -0.244, -0.250, -0.256, -0.262, -0.268, -0.273, -0.279, -0.285, -0.291, -0.297,
    -0.303, -0.309, -0.314, -0.320, -0.326, -0.332, -0.338, -0.343, -0.349, -0.355, -0.361, -0.366,
    -0.372, -0.378, -0.383, -0.389, -0.395, -0.400, -0.406, -0.411, -0.417, -0.423, -0.428, -0.434,
    -0.439, -0.445, -0.450, -0.456, -0.461, -0.466, -0.472, -0.477, -0.483, -0.488, -0.493, -0.499,
    -0.504, -0.509, -0.514, -0.520, -0.525, -0.530, -0.535, -0.540, -0.546, -0.551, -0.556, -0.561,
    -0.566, -0.571, -0.576, -0.581, -0.586, -0.591, -0.596, -0.601, -0.606, -0.611, -0.615, -0.620,
    -0.625, -0.630, -0.635, -0.639, -0.644, -0.649, -0.653, -0.658, -0.663, -0.667, -0.672, -0.676,
    -0.681, -0.685, -0.690, -0.694, -0.698, -0.703, -0.707, -0.711, -0.716, -0.720, -0.724, -0.728,
    -0.733, -0.737, -0.741, -0.745, -0.749, -0.753, -0.757, -0.761, -0.765, -0.769, -0.773, -0.777,
    -0.781, -0.784, -0.788, -0.792, -0.796, -0.799, -0.803, -0.807, -0.810, -0.814, -0.817, -0.821,
    -0.824, -0.828, -0.831, -0.835, -0.838, -0.841, -0.845, -0.848, -0.851, -0.854, -0.858, -0.861,
    -0.864, -0.867, -0.870, -0.873, -0.876, -0.879, -0.882, -0.885, -0.887, -0.890, -0.893, -0.896,
    -0.898, -0.901, -0.904, -0.906, -0.909, -0.911, -0.914, -0.916, -0.919, -0.921, -0.924, -0.926,
    -0.928, -0.931, -0.933, -0.935, -0.937, -0.939, -0.941, -0.943, -0.945, -0.947, -0.949, -0.951,
    -0.953, -0.955, -0.957, -0.958, -0.960, -0.962, -0.964, -0.965, -0.967, -0.968, -0.970, -0.971,
    -0.973, -0.974, -0.976, -0.977, -0.978, -0.979, -0.981, -0.982, -0.983, -0.984, -0.985, -0.986,
    -0.987, -0.988, -0.989, -0.990, -0.991, -0.992, -0.992, -0.993, -0.994, -0.994, -0.995, -0.996,
    -0.996, -0.997, -0.997, -0.998, -0.998, -0.998, -0.999, -0.999, -0.999, -0.999, -1.000, -1.000,
    -1.000, -1.000, -1.000, -1.000, -1.000, -1.000, -1.000, -1.000, -0.999, -0.999, -0.999, -0.999,
    -0.998, -0.998, -0.997, -0.997, -0.996, -0.996, -0.995, -0.995, -0.994, -0.993, -0.993, -0.992,
    -0.991, -0.990, -0.989, -0.988, -0.988, -0.987, -0.986, -0.984, -0.983, -0.982, -0.981, -0.980,
    -0.979, -0.977, -0.976, -0.975, -0.973, -0.972, -0.970, -0.969, -0.967, -0.966, -0.964, -0.963,
    -0.961, -0.959, -0.957, -0.956, -0.954, -0.952, -0.950, -0.948, -0.946, -0.944, -0.942, -0.940,
    -0.938, -0.936, -0.934, -0.931, -0.929, -0.927, -0.925, -0.922, -0.920, -0.917, -0.915, -0.912,
    -0.910, -0.907, -0.905, -0.902, -0.899, -0.897, -0.894, -0.891, -0.889, -0.886, -0.883, -0.880,
    -0.877, -0.874, -0.871, -0.868, -0.865, -0.862, -0.859, -0.856, -0.852, -0.849, -0.846, -0.843,
    -0.839, -0.836, -0.833, -0.829, -0.826, -0.822, -0.819, -0.815, -0.812, -0.808, -0.804, -0.801,
    -0.797, -0.793, -0.790, -0.786, -0.782, -0.778, -0.774, -0.771, -0.767, -0.763, -0.759, -0.755,
    -0.751, -0.747, -0.743, -0.738, -0.734, -0.730, -0.726, -0.722, -0.717, -0.713, -0.709, -0.704,
    -0.700, -0.696, -0.691, -0.687, -0.682, -0.678, -0.673, -0.669, -0.664, -0.660, -0.655, -0.650,
    -0.646, -0.641, -0.636, -0.632, -0.627, -0.622, -0.617, -0.612, -0.608, -0.603, -0.598, -0.593,
    -0.588, -0.583, -0.578, -0.573, -0.568, -0.563, -0.558, -0.553, -0.548, -0.542, -0.537, -0.532,
    -0.527, -0.522, -0.516, -0.511, -0.506, -0.501, -0.495, -0.490, -0.485, -0.479, -0.474, -0.469,
    -0.463, -0.458, -0.452, -0.447, -0.441, -0.436, -0.430, -0.425, -0.419, -0.414, -0.408, -0.402,
    -0.397, -0.391, -0.385, -0.380, -0.374, -0.368, -0.363, -0.357, -0.351, -0.346, -0.340, -0.334,
    -0.328, -0.322, -0.317, -0.311, -0.305, -0.299, -0.293, -0.287, -0.282, -0.276, -0.270, -0.264,
    -0.258, -0.252, -0.246, -0.240, -0.234, -0.228, -0.222, -0.216, -0.210, -0.204, -0.198, -0.192,
    -0.186, -0.180, -0.174, -0.168, -0.162, -0.156, -0.150, -0.144, -0.138, -0.132, -0.126, -0.120,
    -0.114, -0.108, -0.101, -0.095, -0.089, -0.083, -0.077, -0.071, -0.065, -0.059, -0.053, -0.046,
    -0.040, -0.034, -0.028, -0.022, -0.016, -0.010};

class WaveformComponent : public Component,
//public Buffer::Listener,
public ComponentListener
{
public:
    WaveformComponent(/*Buffer& bufferToControl*/);
    ~WaveformComponent();
    
    void resized();
    void paint (Graphics& g);
    void bufferChanged();
    void componentMovedOrResized(Component& component, bool wasMoved, bool wasResized);
    void resetBuffer();
    void refreshPath(int lfo_wave);
    
private:
    enum CurvePoints
    {
        pointX1,
        pointY1,
        pointX2,
        pointY2,
        pointX3,
        pointY3,
        numPoints,
    };
    
    Path path;
    
    OwnedArray<CurvePoint> curvePoints;
    OwnedArray<Value> values;
    
    void refillBuffer(float x1, float y1, float x2, float y2, float x3, float y3);
    void resetPoints();
    
    Image background;
    bool isInitialised;
    bool initBuffer;
    bool secondTime;
    
    LFOWaveformTable lfo;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveformComponent);
};

#endif /* defined(__NebuloMod__WaveformComponent__) */
