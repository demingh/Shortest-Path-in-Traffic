// main.cpp
//
// ICS 46 Spring 2017
// Project #4: Rock and Roll Stops the Traffic
//
// This is the program's main() function, which is the entry point for your
// console user interface.

#include "InputReader.hpp"
#include "RoadMapReader.hpp"
#include "TripReader.hpp"
#include "RoadMapWriter.hpp"
#include "RoadMap.hpp"
#include <iostream> 
#include <iomanip> 


void show_time(double total_time)
{
	double hours=0; 
	double minutes=0; 
	double seconds=0; 
	minutes=total_time/60; 
	hours=minutes/60; 
	seconds=(minutes-static_cast<int>(minutes))*60; 
	if(static_cast<int>(hours)==0&&static_cast<int>(minutes)!=0)
	{
		std::cout<<static_cast<int>(minutes)<<" mins "<<std::setprecision(2)<<std::fixed<<seconds<<" secs";
	}
	else if(static_cast<int>(hours)==0&&static_cast<int>(minutes)==0)
	{
		std::cout<<seconds<<" secs";
	}
	else
	{
		std::cout<<static_cast<int>(hours)<<" hrs "<<static_cast<int>(minutes)<<" mins "<<std::setprecision(2)<<std::fixed
		<<seconds<<" secs";
	}

}
	double get_distance(RoadSegment Road)
	{
		return Road.miles; 
	}

	std::function<double(const RoadSegment&)> shortest_path=get_distance;

	double get_time(RoadSegment Road)
	{
		return Road.miles/Road.milesPerHour; 
	}

	std::function<double(const RoadSegment&)> shortest_time=get_time; 

struct records
{
	int vertex; 
	std::string location; 
	RoadSegment info;
};

	
int main()
{	
	
	InputReader ir(std::cin);
	RoadMap Graph;
	RoadMapReader roadreader; 
	Graph=roadreader.readRoadMap(ir);

	/*RoadMapWriter roadmapwriter;
	roadmapwriter.writeRoadMap(std::cout,Graph);*/
	
	TripReader tripreader;
	std::vector<Trip> trips = tripreader.readTrips(ir); 


	std::map<int,int> route; 
	for(auto single_trip:trips)
	{
		std::vector<records> target_trip; 
		if(single_trip.metric==TripMetric::Distance)
		{
			route=Graph.findShortestPaths(single_trip.startVertex,shortest_path);
			int destination=single_trip.endVertex; 
			records dest;
			dest.vertex=destination; 
			dest.location=Graph.vertexInfo(destination);
			dest.info=Graph.edgeInfo(route[destination],destination);
			target_trip.push_back(dest);
			while(single_trip.startVertex!=destination)
			{
				int next_stop=destination;
				destination=route[destination];
				records trip_record; 
				trip_record.vertex=destination;
				trip_record.location=Graph.vertexInfo(destination);
				trip_record.info=Graph.edgeInfo(destination,next_stop);
				target_trip.push_back(trip_record);
			}
			std::cout<<"Shortest distance from "<<Graph.vertexInfo(target_trip.back().vertex)<<" to "
			<<Graph.vertexInfo(target_trip.front().vertex)<<"\n   Begin at "<<target_trip.back().location<<"\n";
			double distance=0.0;
			for(int n=target_trip.size()-2;n>-1; --n)
			{
				std::cout<<"   Countinue to "<<target_trip[n].location<<" ("<<std::setprecision(2)<<std::fixed<<target_trip[n+1].info.miles<< " miles)\n";
				distance+=target_trip[n+1].info.miles; 

			}
			std::cout<<"Total Distance: "<<std::setprecision(2)<<std::fixed<<distance<<" miles\n\n\n";
		}
		else
		{
			route=Graph.findShortestPaths(single_trip.startVertex,shortest_time);
			int destination=single_trip.endVertex; 
			records dest;
			dest.vertex=destination; 
			dest.location=Graph.vertexInfo(destination);
			dest.info=Graph.edgeInfo(route[destination],destination);
			target_trip.push_back(dest);
			 
			while(single_trip.startVertex!=destination)
			{
				int next_stop=destination;
				destination=route[destination];
				records trip_record; 
				trip_record.vertex=destination;
				trip_record.location=Graph.vertexInfo(destination);
				trip_record.info=Graph.edgeInfo(destination,next_stop);
				target_trip.push_back(trip_record);
			}

			std::cout<<"Shortest driving time from "<<Graph.vertexInfo(target_trip.back().vertex)
			<<" to "<<Graph.vertexInfo(target_trip.front().vertex)<<"\n";
			std::cout<<"   Begin at "<< target_trip.back().location<<"\n";
			double time =0.0;
			for(int n=target_trip.size()-2; n>-1; --n)
			{
				double time_in_hour=(target_trip[n+1].info.miles/target_trip[n+1].info.milesPerHour);
				double time_in_second=time_in_hour*3600; 
				std::cout<<"   Continue to "<<target_trip[n].location<< "("<<std::setprecision(2)<<std::fixed<<target_trip[n+1].info.miles<<
				" miles @ "<<target_trip[n+1].info.milesPerHour<<" mph"<<" = ";
				show_time(time_in_second);
				std::cout<<")\n"; 
				time+=time_in_second;
			}
			std::cout<<"Total time: ";
			show_time(time);
			std::cout<<"\n\n\n";

		}

	}
    return 0;
}

