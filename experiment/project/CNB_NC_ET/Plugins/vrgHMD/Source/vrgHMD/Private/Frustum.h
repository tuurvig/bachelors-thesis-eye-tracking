#pragma once

struct FMatrix;

void CreateOurFrustumUnreal(float lhFOV, float rhFOV, float vFOVhalf, float n, float f, float rot, FMatrix& Res);

void CreateOurFrustumUnreal2(float left, float right, float top, float bottom, float n, float f, FMatrix& Res);