/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 *
 * Copyright (c) 2016-2018 Inviwo Foundation
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *********************************************************************************/

layout(location = 4) in uint in_PickId;

#include "plotting/common.glsl"
#include "utils/structs.glsl"
#include "utils/pickingutils.glsl"

uniform vec2 minmaxX;
uniform vec2 minmaxY;

uniform float maxRadius;

uniform bool pickingEnabled = false;

out vec4 vColor;
out float vRadius;
out float vDepth;
flat out vec4 pickColors_;

float norm(in float v, in vec2 mm) { 
    return (v - mm.x) / (mm.y - mm.x); 
}

void main(void) {   
    vColor = in_Color;
    vRadius = maxRadius;
    vDepth = 0.5;
    
    vec2 point = vec2(norm(in_Vertex.x, minmaxX), norm(in_Vertex.y, minmaxY));
    gl_Position = vec4(point, 0.5, 1.0);
    pickColors_ = vec4(pickingIndexToColor(in_PickId), pickingEnabled ? 1.0 : 0.0);
}