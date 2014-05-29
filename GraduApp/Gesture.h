#pragma once
#include <list>

namespace SST
{
	struct Data
	{
		Data() : mX(0.000), mY(0.000), mZ(0.000) { }
		Data(double x, double y, double z) : mX(x), mY(y), mZ(z) { }
		double mX;
        double mY;
        double mZ;
        int mCluster;
	};

	struct Centroid
	{
		Centroid() : mX(0.0), mY(0.0), mZ(0.0) { }
		Centroid(double x, double y, double z) : mX(x), mY(y), mZ(z) { }
		double mX;
        double mY;
        double mZ;
	};

	class Gesture
	{
	public:
		Gesture();
		void AddPoint(DirectX::XMFLOAT3 _point);
		std::list<DirectX::XMFLOAT3> GetGesture();

		void kMeanCluster();
		
	private:
		DirectX::XMFLOAT3 m_vector; //one vector point
		std::list<DirectX::XMFLOAT3> m_samples; // collection of vector points = actual gesture

		
		static const int NUM_CLUSTERS = 16;    // Total clusters.
        //static const int TOTAL_DATA = 4;      // Total data points.

		double dist(Data d, Centroid c);
		std::list<Centroid> m_centroids;
		std::list<Data> m_dataset;
	};
}