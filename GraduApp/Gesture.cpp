#include "pch.h"
#include "Gesture.h"

#include <sstream>

#define _USE_MATH_DEFINES
#include <math.h>


namespace SST
{
	Gesture::Gesture()
	{
		// Initialize cluster centroids
		double vl = 0.1;
		m_centroids.push_back(Centroid(0.0, 0.0, 0.0));

		m_centroids.push_back(Centroid(vl, 0.0, 0.0));
		m_centroids.push_back(Centroid(-vl, 0.0, 0.0));

		m_centroids.push_back(Centroid(0.0, vl, 0.0));
		m_centroids.push_back(Centroid(0.0, -vl, 0.0));

		m_centroids.push_back(Centroid(0.0, 0.0, vl));
		m_centroids.push_back(Centroid(0.0, 0.0, -vl));

		double coord = sin(M_PI/4)/10;
		m_centroids.push_back(Centroid(coord, coord, 0.0));
		m_centroids.push_back(Centroid(coord, -coord, 0.0));

		m_centroids.push_back(Centroid(-coord, coord, 0.0));
		m_centroids.push_back(Centroid(-coord, -coord, 0.0));

		m_centroids.push_back(Centroid(0.0, -coord, coord));
		m_centroids.push_back(Centroid(0.0, -coord, -coord));

		m_centroids.push_back(Centroid(0.0, coord, coord));
		m_centroids.push_back(Centroid(0.0, coord, -coord));
	}

	
	void Gesture::AddPoint(DirectX::XMFLOAT3 _point)
	{
		m_samples.push_back(_point);
	}

	std::list<DirectX::XMFLOAT3> Gesture::GetGesture()
	{
		return m_samples;
	}

	double Gesture::dist(Data d, Centroid c)
    {
        return sqrt(pow((c.mX - d.mX), 2) + pow((c.mY - d.mY), 2) + pow((c.mZ - d.mZ), 2));
    }

	void Gesture::kMeanCluster()
    {
        double bigNumber = pow(10, 10);    // some big number that's sure to be larger than our data range.
        double minimum = bigNumber;        // The minimum value to beat. 
        double distance = 0.0;             // The current minimum value.
        int sampleNumber = 0;
        int cluster = 0;
        boolean isStillMoving = true;
        Data newData;
        
		/*double samples[4][3] = {{1.0, 0.0, 0.0}, 
                                {-0.2, 0.0, 0.0}, 
                                {0.0, 0.3, 0.8}, 
                                {0.0, 1.0, 0.0}};
		*/

        // Add in new data, one at a time, recalculating centroids with each new one. 
		for(std::list<DirectX::XMFLOAT3>::iterator s_it = m_samples.begin(); s_it != m_samples.end(); ++s_it)
        //while(m_dataset.size() < TOTAL_DATA)
        {
            //newData = Data(samples[sampleNumber][0], samples[sampleNumber][1], samples[sampleNumber][2]);
			newData = Data(s_it->x, s_it->y, s_it->z);
						double test = newData.mX;

            m_dataset.push_back(newData);
            minimum = bigNumber;
			int i = 0;
			for(std::list<Centroid>::iterator c_it = m_centroids.begin(); c_it != m_centroids.end(); ++c_it)
            {
                distance = dist(newData, *c_it);
                if(distance < minimum){
                    minimum = distance;
                    cluster = i;
                }
				i++;
            }
			m_dataset.back().mCluster = cluster;
			//newData.mCluster = cluster;
            
            // calculate new centroids.
			i = 0;
			for(std::list<Centroid>::iterator c_it = m_centroids.begin(); c_it != m_centroids.end(); ++c_it)
            {
                double totalX = 0;
                double totalY = 0;
				double totalZ = 0;
                int totalInCluster = 0;

                for(unsigned int j = 0; j < m_dataset.size(); j++)
                {
					std::list<Data>::iterator d_it = m_dataset.begin();
					for(unsigned int k = 0; k < j; ++k) { d_it++; }

                    if(d_it->mCluster == i){
                        totalX += d_it->mX;
                        totalY += d_it->mY;
                        totalZ += d_it->mZ;
                        totalInCluster++;
                    }
                }

                if(totalInCluster > 0){
                    c_it->mX = totalX / totalInCluster;
                    c_it->mY = totalY / totalInCluster;
                    c_it->mZ = totalZ / totalInCluster;
                }
				i++;
            }
            sampleNumber++;
        }
        
        // Now, keep shifting centroids until equilibrium occurs.
        while(isStillMoving)
        {
            // calculate new centroids.
			int i = 0;
			for(std::list<Centroid>::iterator c_it = m_centroids.begin(); c_it != m_centroids.end(); ++c_it)
            {
                double totalX = 0;
                double totalY = 0;
				double totalZ = 0;
                int totalInCluster = 0;
                for(unsigned int j = 0; j < m_dataset.size(); j++)
                {
					std::list<Data>::iterator d_it = m_dataset.begin();
					for(unsigned int k = 0; k < j; ++k) { d_it++; }
                    if(d_it->mCluster == i){
                        totalX += d_it->mX;
                        totalY += d_it->mY;
                        totalZ += d_it->mZ;
                        totalInCluster++;
                    }
                }
                if(totalInCluster > 0){
                    c_it->mX = totalX / totalInCluster;
                    c_it->mY = totalY / totalInCluster;
                    c_it->mZ = totalZ / totalInCluster;
                }
				i++;
            }
            
            // Assign all data to the new centroids
            isStillMoving = false;
            
            for(unsigned int i = 0; i < m_dataset.size(); i++)
            {
				std::list<Data>::iterator d_it = m_dataset.begin();
				for(int k = 0; k < i; ++k) { d_it++; }
                Data tempData(d_it->mX, d_it->mY, d_it->mZ);
				tempData.mCluster = d_it->mCluster;

                minimum = bigNumber;
				int j = 0;
				for(std::list<Centroid>::iterator c_it = m_centroids.begin(); c_it != m_centroids.end(); ++c_it)
				{
                    distance = dist(tempData, *c_it);
                    if(distance < minimum){
                        minimum = distance;
                        cluster = j;
                    }
					j++;
                }
                tempData.mCluster = cluster;
                if(tempData.mCluster != cluster) {
                    tempData.mCluster = cluster;
                    isStillMoving = true;
                }
            }
        }
        
		
		for(std::list<Centroid>::iterator c_it = m_centroids.begin(); c_it != m_centroids.end(); ++c_it)
		{
			double test1 = c_it->mX;
			double test2 = c_it->mY;
			double test3 = c_it->mZ;
		}
		int test_size = m_dataset.size();

		for(std::list<Data>::iterator d_it = m_dataset.begin(); d_it != m_dataset.end(); ++d_it)
		{
			double x = d_it->mX;
			double y = d_it->mY;
			double z = d_it->mZ;
			int test = d_it->mCluster;

			// DEBUG PRINT
			std::ostringstream sstream2;
				sstream2 << "<Gesture::k-means> x=" << x << " y=" << y << " z=" << z << " cluster=" << test << "\n";
			std::string s2 = sstream2.str();
			OutputDebugStringA(s2.c_str());
		
		}
    }
}