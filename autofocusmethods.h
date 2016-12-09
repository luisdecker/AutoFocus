#ifndef AUTOFOCUSMETHODS_H
#define AUTOFOCUSMETHODS_H
#include <opencv.hpp>
#include <map>
#include<float.h>
#include <vector>

#include "focusmetrics.h"
#include "hologramdecoder.h"

#define DECODE_WAIT_TIME 0

namespace AFM {
    //Normalize the image
    inline cv::Mat
    normalize( cv::Mat image, double minVal, double maxVal ) {
        image = image - minVal;
        image = image / maxVal;
        return image;
    }
    /****************************************
     * CLASSIC
     * *************************************/


	/*Classic autofocus method (photographic camera method)*/
	class Classic {

	public:
        //Constructor
		Classic( cv::Mat image, cv::Mat ROI = cv::Mat() , FM::FocusMetric * focusMetric = 0 );

        //Returns the plane of focus
        double
        find_focus( cv::Mat image = cv::Mat(), cv::Mat ROI = cv::Mat(), FM::FocusMetric * metric = 0, double min=-1, double max = -1 );

        //Get the image in focus
		cv::Mat
        operator()() {
            cv::Mat hologram = decode_to( this->ROI, find_focus() );
           return split_hologram(hologram);
        }

	private:

        //Display a hologram in a window
		static void
		show_hologram( cv::Mat image, int delay = 1 );

        //Split the real values of the hologram
		static cv::Mat
		split_hologram( cv::Mat image );

        //Decode a hologram to given focal point
		cv::Mat
		decode_to( cv::Mat image, int focalPoint );

        //Find maximum and minimum values for normalization
        std::pair<double, double>
        findMinMax( cv::Mat image );

        //___________________________________________________________

        //The full image and the region of interest
		cv::Mat
		image, ROI;

        //The chosen focus metric
		FM::FocusMetric *
		focusMetric;

		//Minimal and maximal values to try to find the focus.
		int
        minDecode = 200, maxDecode = 15000;

		//Step between focus planes;
		int
        step = 200;

	};

    /****************************************
     * COMPARATIVE
     * *************************************/

	/* Test all the implemented focus methods.*/
	class Comparative {
		/*
		    This class aims to test all the implemented focus metrics.
		    It writes a file ( $PWD/results/reports/comparativeReports.csv)
		in this format:

		RecontructionPlane ; Method1 Measured Value; Method 2 M.V; ... ; Method n M.V.
		0                  ;         100           ;     150     ; ... ;     110
		100                ;         150           ;     170     ; ... ;     140
		...


		*/
	public:
		Comparative( cv::Mat image,
					 int threshold = 50,
                     int minDecode = 200,
					 int maxDecode = 15000,
                     int step = 200 );

        //Operator (), returns a comparative table with the detected focus planes, or with the measures
        //at each plane.
        std::string
        operator()( bool focusPlane );

		//A table with all the measured metrics
		typedef std::vector < std::vector  <double > >
		MetricsTable;
	private:
		//Prepare the image to the algorithm
		cv::Mat
		prepare_image( cv::Mat img );

		//Compare all methods, and returns the result of every method in every recontruction plane.
		MetricsTable
		create_measure_table();

		//Checks image integrity
		void
		check_image( cv::Mat image );

		//Decode the image to the desired hologram plane
		cv::Mat
		decode_to( cv::Mat input, int plane , bool complexChannel );

		//Generate a CSV string from a comparison table
        std::string
        generate_csv( );

		//Generate a vector with all the used recontrunction distances
        std::vector<int>
        generate_distances();

        //Genereate min and max values
        void
        calculateMinMax();

        //Generate a string with the measured focus from ever method
        std::string
        create_points_table();

        //Gets the focal plane detected adby given metric
        double
        get_focal_plane( FM::FocusMetric * metric );

		//___________________________________________________________
		//All the focus metrics used to comparison.
		std::vector<FM::FocusMetric *>
		metrics;

		//The minimum reconstruction range
		int
		minDecode;

		//The maximum reconstruction range
		int
		maxDecode;

		//The step of reconstruction
		int
		step;

		//Image to analise;
		cv::Mat image;

        //Minimum and maximum reconstruction values
        double
        minVal, maxVal;

        //Minimum and maximum calculated
        bool
        haveMinMax;

        //Ordered name of the metrics
        const std::string
        methodsNames[10] = {"Treshold Gradient-Squared", "Thresholded Brenner Gradient" ,
                            "Tenenbaum Gradient", "Energy of Laplacian", "Thresholded Histogram Range",
                            "Normalized Variance", "Auto Corretalion", "Standard deviation based correlation" ,
                            "Image Power" , "Thresholded pixel-count"
                           };
    };

}
#endif // AUTOFOCUSMETHODS_H
