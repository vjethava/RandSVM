/***************************************************************************
 *   Copyright (C) 2005 by Krishnan S                                      *
 *   krishi@csa.iisc.ernet.in                                              *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include <iostream>
#include <fstream>
#include <sstream>
#include "lpsolve.h"

using namespace std;

/*LPSolve::LPSolve(ulong constraints, ulong features):no_of_constraints(constraints), no_of_features(features)
{
	soln = new REAL[no_of_features+1];
	obj  = new REAL[2*no_of_features+2];
	for( ulong i = 1; i <= 2*no_of_features; i++ )
		obj[i] = 1;
	obj[2*no_of_features+1] = 0;
}

void LPSolve::Learn(string ifile)
{
	lprec *lp = make_lp(0,2*no_of_features+1);
	if( lp == NULL )
	{
		//Probably not enough memory 
		cerr << "Unable to create LP model" << endl;
		exit(1);
	}
	
	ifstream in(ifile.c_str());
	if( !in.good() )
	{
		cerr << "LPSolve:: Unable to open file " << ifile << endl;
		return;
	}
	
	set_obj_fn(lp, obj);
	REAL *row;
	ulong count;
	int *colno;
	row = new REAL[2*no_of_features+1];
	colno = new int[2*no_of_features+1];
	
	
	for( int i = 0; i < no_of_constraints; i++ )
	{
		string line;
		getline(in, line);
		
		istringstream lss(line);
		
		int category;
		lss >> category;
		
		count = 0;
		while( !lss.eof() )
		{
			string feature_weight;
			lss >> feature_weight;
			
			//if( lss.eof() )
			//	break;
			
			string::size_type separator_pos = feature_weight.find(":", 0);
			feature_weight.replace(separator_pos, 1, " ");
			
			int feature;
			double weight;
			istringstream(feature_weight) >> feature >> weight;
			
			row[count] = category*weight;
			row[count+1] = -1*category*weight;
			colno[count] = 2*feature-1;
			colno[count+1] = 2*feature;
			count+=2;
		}
		//Variable b
		row[count] = category;
		colno[count] = 2*no_of_features+1;
		count++;
		
		if( !add_constraintex(lp, count, row, colno, GE, 1.0) )
			cerr << "LPSolve:: Unable to add constraint" << endl; 
	}
	
	set_unbounded(lp, 2*no_of_features+1 );	
	solve(lp);
	
	REAL *lp_soln = new REAL[2*no_of_features+1];
	
	
	get_variables(lp, lp_soln);
	
	for( ulong i = 0; i < no_of_features*2 ; i+=2  )
		soln[i/2] = lp_soln[i] - lp_soln[i+1];
	soln[0] = lp_soln[2*no_of_features];
	
	delete []lp_soln;
	delete_lp(lp);


}

ulong LPSolve::Classify(string tfile, bool *categories)
{
	ifstream in(tfile.c_str());
	if( !in.good() )
	{
		cerr << "Unable to open test file " << tfile << endl;
		exit(2);
	}
	
	ulong ctr=0, no_of_violators=0;
	while( !in.eof() )
	{
		string line;
		getline(in, line);
		
		istringstream lss(line);
		
		int category;
		lss >> category;

		double sum = 0;
	
		bool features_present = false;
	
		while( !lss.eof() )
		{
			string feature_weight;
			lss >> feature_weight;
			
			string::size_type separator_pos = feature_weight.find(":", 0);
			feature_weight.replace(separator_pos, 1, " ");
				
			ulong feature;
			double weight;
			istringstream(feature_weight) >> feature >> weight;
			
			sum += soln[feature]*weight;
			features_present = true;
		}
	
		sum += soln[0];
	
		if( !features_present )
		{
			no_of_violators++;
			categories[ctr++] = false;
			continue;
		}
	
		if( sum > 0 )
			sum = 1;
		else
			sum = -1;
	
		if( sum == category )
			categories[ctr++] = true;
		else
		{
			no_of_violators++;
			categories[ctr++] = false;
		}
	}
	return no_of_violators;
}

void LPSolve::SaveModel(string mfile)
{
	ofstream out;
	out.open(mfile.c_str() );
	if( !out.good() )
	{
		cerr << "Unable to open model file" << endl;
		exit(1);
	}	
	out << "#Number of features" << endl << no_of_features  << endl << "#w" << endl;

	for( int i = 0; i < no_of_features+1 ; i+=1  )
		out << soln[i] << endl;

	out << "#b" << endl << soln[no_of_features+1] << endl;

}

LPSolve::~LPSolve()
{
	delete []soln;
	delete []obj;
}*/


