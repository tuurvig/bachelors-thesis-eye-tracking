#include "Frustum.h"

#include "Math/Matrix.h"
#include "Math/Vector.h"

#include <cmath>

namespace {

FVector mulmatvec(const FMatrix& A, const FVector& b) {
    FVector res;
    res.X = A.M[0][0] * b.X + A.M[0][1] * b.Y + A.M[0][2] * b.Z;
    res.Y = A.M[1][0] * b.X + A.M[1][1] * b.Y + A.M[1][2] * b.Z;
    res.Z = A.M[2][0] * b.X + A.M[2][1] * b.Y + A.M[2][2] * b.Z;
    return res;
}

void CreateOffAxisFrustumUnreal(float l, float r, float t, float b, float n, FMatrix& Res) {
    Res.M[0][0] = (2.0f * n) / (r - l);
    Res.M[0][1] = 0;
    Res.M[0][2] = 0;
    Res.M[0][3] = 0;
    Res.M[1][0] = 0;
    Res.M[1][1] = (2.0f * n) / (t - b);
    Res.M[1][2] = 0;
    Res.M[1][3] = 0;
    Res.M[2][0] = -(r + l) / (r - l);
    Res.M[2][1] = -(t + b) / (t - b);
    Res.M[2][2] = 0;
    Res.M[2][3] = 1.0f;
    Res.M[3][0] = 0;
    Res.M[3][1] = 0;
    Res.M[3][2] = n;
    Res.M[3][3] = 0;
}

void ArbitraryFrustumUnreal(const FVector& pa, const FVector& pb, const FVector& pc, const FVector& pe, float n, float f, FMatrix& Res) {
    FVector vr = pb - pa;
    FVector vu = pc - pa;

    vr.Normalize();
    vu.Normalize();

    FVector vn = FVector::CrossProduct(vr, vu);
    vn.Normalize();

    FVector va = pa - pe;
    FVector vb = pb - pe;
    FVector vc = pc - pe;

    auto d = -FVector::DotProduct(vn, va);
    auto l = FVector::DotProduct(vr, va) * n / d;
    auto r = FVector::DotProduct(vr, vb) * n / d;
    auto b = FVector::DotProduct(vu, va) * n / d;
    auto t = FVector::DotProduct(vu, vc) * n / d;

    FMatrix P;
    CreateOffAxisFrustumUnreal(l, r, t, b, n, P);

    FMatrix M;
    M.M[0][0] = vr[0];
    M.M[0][1] = vr[1];
    M.M[0][2] = vr[2];
    M.M[0][3] = 0;
    M.M[1][0] = vu[0];
    M.M[1][1] = vu[1];
    M.M[1][2] = vu[2];
    M.M[1][3] = 0;
    M.M[2][0] = vn[0];
    M.M[2][1] = vn[1];
    M.M[2][2] = vn[2];
    M.M[2][3] = 0;
    M.M[3][0] = 0;
    M.M[3][1] = 0;
    M.M[3][2] = 0;
    M.M[3][3] = 1.0f;

    Res = P * M;
}

void rotationmatFromAxisAngle(const FVector& axis, float angle, FMatrix& R) {
    float cosa = cosf(FMath::DegreesToRadians(angle));
    float sina = sinf(FMath::DegreesToRadians(angle));

    R.M[0][0] = cosa + axis.X * axis.X * (1 - cosa);
    R.M[0][1] = axis.X * axis.Y * (1 - cosa) - axis.Z * sina;
    R.M[0][2] = axis.X * axis.Z * (1 - cosa) + axis.Y * sina;
    R.M[0][3] = 0;
    R.M[1][0] = axis.Y * axis.X * (1 - cosa) + axis.Z * sina;
    R.M[1][1] = cosa + axis.Y * axis.Y * (1 - cosa);
    R.M[1][2] = axis.Y * axis.Z * (1 - cosa) - axis.X * sina;
    R.M[1][3] = 0;
    R.M[2][0] = axis.Z * axis.X * (1 - cosa) - axis.Y * sina;
    R.M[2][1] = axis.Z * axis.Y * (1 - cosa) + axis.X * sina;
    R.M[2][2] = cosa + axis.Z * axis.Z * (1 - cosa);
    R.M[2][3] = 0;
    R.M[3][0] = 0;
    R.M[3][1] = 0;
    R.M[3][2] = 0;
    R.M[3][3] = 1;
}

} // namespace

void CreateOurFrustumUnreal(float lhFOV, float rhFOV, float vFOVhalf, float n, float f, float rot, FMatrix& Res) {
    FVector pa(n * tanf(lhFOV), n * tanf(-vFOVhalf), 0);
    FVector pb(n * tanf(rhFOV), n * tanf(-vFOVhalf), 0);
    FVector pc(n * tanf(lhFOV), n * tanf(vFOVhalf), 0);

    FVector axis(0, 1, 0);

    FMatrix R;
    rotationmatFromAxisAngle(axis, rot, R);
    pa = mulmatvec(R, pa);
    pb = mulmatvec(R, pb);
    pc = mulmatvec(R, pc);
    pa.Z -= n;
    pb.Z -= n;
    pc.Z -= n;
    FVector pe(0, 0, 0);
    ArbitraryFrustumUnreal(pa, pb, pc, pe, n, f, Res);
}
