namespace RaytracingEngine.extensions;

public static class Algorithms
{
    public static double StratifiedMonteCarloIntegration(Func<double, double> function, double start, double end,
        int pointCount, int stratCount)
    {
        double sum = 0;
        double intervalLength = (end - start) / stratCount;

        for (int i = 0; i < stratCount; i++)
        {
            double localA = start + i * intervalLength;
            double localB = localA + intervalLength;

            sum += MonteCarloIntegration(function, localA, localB, pointCount);
        }

        return sum;
    }

    public static double ImportanceSamplingMonteCarloIntegration(Func<double, double> function, double start, double end, int pointCount, Func<double, double> pdf)
    {
        Random random = new Random();
        double sum = 0;

        for (int i = 0; i < pointCount; i++)
        {
            double x = start + (end - start) * random.NextDouble();
            double y = function(x) / pdf(x);
            sum += y;
        }

        return (end - start) * sum / pointCount;
    }
}