using System.Numerics;

namespace RaytracingEngine.extensions;

public class RandomHelper
{
    private static readonly Random _random = new();

    
    public static double RandomDouble() => _random.NextDouble();

    public static Ray RandomRay(Vector3 origin, Vector3 normal)
    {
        var heightRandom = _random.NextDouble() * 2 * -1;
        var phiRandom = _random.NextDouble() * 2 * Math.PI;

        var r = normal.Z != 0 ? new Vector3(normal.Y, -normal.X, 0) : new Vector3(0, -normal.Z, normal.Y);

        var rotation = Quaternion.CreateFromAxisAngle(normal, (float)phiRandom);
        var transformedDirection = Vector3.Transform(r, rotation);

        return new Ray(origin, normal * (float)heightRandom + transformedDirection);
    }
}