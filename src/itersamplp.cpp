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
#include "itersamplp.h"
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstring>

using namespace std;

/*ITERSAMPLP::ITERSAMPLP(const string input_file, const string output_file, const string model_file, const string learner_cmd, const string learner_opts, const string classifier_cmd, const ulong no_of_variables,  const size_t no_of_constraints, const size_t sample_size, const double err, const size_t memory_size, SAMPLER &s): RND_SVM(input_file, no_of_variables, no_of_constraints, sample_size, memory_size, err, s), mfile(model_file), ofile(output_file), learner_command(learner_cmd), learner_options(learner_opts), classifier_command(classifier_cmd), lp_solver(sample_size, no_of_variables)
{
	//InitializeLearnerArgs();	
	//InitializeClassifierArgs();
}

void ITERSAMPLP::InitializeLearnerArgs()
{
	learner_argv = new char *[8];

 	learner_argv[0] = "LPLearn";

	learner_argv[1] = "-i";
	learner_argv[2] = new char[ sfile.length() + 1 ];
	strcpy(learner_argv[2], sfile.c_str());

	learner_argv[3] = "-o";
	learner_argv[4] = new char[ mfile.length() + 1 ];
	strcpy(learner_argv[4], mfile.c_str() );

	learner_argv[5] = "-d";
	ostringstream feature_count_str;
	feature_count_str << no_of_variables;
	learner_argv[6] = new char[ feature_count_str.str().length() + 1 ];
	strcpy(learner_argv[6], feature_count_str.str().c_str() );
	
	
	learner_argv[7] = "0";
	
}

void ITERSAMPLP::InitializeClassifierArgs()
{
	classifier_argv = new char *[8];
	classifier_argv[0] = "LPClassify";

	classifier_argv[1] = "-t";
	classifier_argv[2] = new char[ ifile.length() + 1 ];
	strcpy(classifier_argv[2], ifile.c_str());
	
	classifier_argv[3] = "-m";
	classifier_argv[4] = new char[ mfile.length() + 1 ];
	strcpy(classifier_argv[4], mfile.c_str()) ;
	
	classifier_argv[5] = "-o";
	classifier_argv[6] = new char[ ofile.length() + 1 ];
	strcpy(classifier_argv[6], ofile.c_str() );
	
	classifier_argv[7] = 0;
}

void ITERSAMPLP::Solve()
{
	pid_t cpid;
	
	lp_solver.Learn(sfile);
	solver_failed = false;

	/*if( (cpid = fork() ) == 0 )
	{
		if( execv( learner_command.c_str(), learner_argv ) )
			perror("ITERSAMPLP::Solve - execv()");	
		return;
	}	
	else if( cpid == -1 )
	{
		perror("ITERSAMPLP::Solve - fork()");
		solver_failed = true;
		return;
	}

	int status;
	wait( &status );

	if( !WIFEXITED(status) )
	{
		cerr << "ITERSAMPLP::Solve : svm learner exited abnormally" << endl;
		solver_failed = true;
		return;
	}*/

//}

//size_t ITERSAMPLP::FindViolators(const bool *sampled_constr) 
//{
	/*pid_t cpid;
	
		
	if( (cpid = fork() ) == 0 )
	{
		if( execv( classifier_command.c_str(), classifier_argv ) )
			perror("ITERSAMPLP::FindViolators");
		return 0;
	}
	else if( cpid == -1 )
	{
		perror("ITERSAMPLP::FindViolators");
		return 0;
	}
	
	int status;
	wait( &status );

	if( !WIFEXITED(status) )
	{
		cerr << "ITERSAMPLP::FindViolators : svm classifier exited abnormally" << endl;
		return 0;
	}
	
	ifstream in( ofile.c_str() );

  	if( !in.good() )
	{
		cerr << "ITERSAMPLP::FindViolators : Unable to open file " << ofile << endl;
		return 0;
	}	

	size_t no_of_violators = 0;
	size_t sv_ctr = 0;
	no_of_sv = 0;
	cout << "Checking for violations" << endl;
	for( ulong i = 0; i < no_of_constraints; i++ )
	{
		violators[i] = false;
				
		int category;
		in >> category;
					
		category = category > 0 ? true : false;
		if( category != categories[i] )
 		{
			violators[i] = true;
			no_of_violators++;
		}
	}	
	cout << "Violators check done" << endl;
	//delete []alphas;
	return no_of_violators;*/
/*	ulong no_of_violators = lp_solver.Classify(ifile, violators);
	lp_solver.SaveModel(mfile);
	return no_of_violators;
}

ITERSAMPLP::~ITERSAMPLP()
{
}*/
