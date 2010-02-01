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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <iostream>
#include <cstdlib>
#include <getopt.h>
#include <sstream>
#include <sys/resource.h>
#include <sys/time.h>
#include <unistd.h>
#include "rnd_libsvm.h"
#include "rnd_svmlight.h"
#include "itersamplp.h"




using namespace std;

#define SVMLight 0
#define LIBSVM   1
#define LPSolve	 2

#ifdef DEBUG
void Solve(const ulong, const size_t, const size_t, const string,  const string, const string, const string, const size_t, const double, const int, const string, const string, const string, problem_type = classification, double = -1 );
#else
void Solve(const ulong, const size_t, const size_t, const string,  const string, const string, const string, const size_t, const double, const int, problem_type pt = classification, double eps = -1  );
#endif

int main(int argc, char *argv[])
{

	string ifile="", mfile="", bindir="", svm_options="";
#ifdef DEBUG
	string sv_file="", weights_file="", violators_file="";
#endif
	ulong no_of_variables = 0;
	size_t no_of_constr  = 0, sample_size = 0, memory_size = 0;
	int c;
	int option_index = 0;
	int solver_name = SVMLight;
	double err = .1;
	double eps = -1;
	problem_type pt=classification;
	string t;
	
	struct option long_options[] = { {"bindir",1,0,1}, {"svmoptions",1,0,2}, {0, 0, 0, 0} };
	
	while(1)
	{
		c = getopt_long(argc, argv, "z:p:n:d:s:b:m:t:e:h", long_options, &option_index );
		
		if( c == -1 )
			break;
		
		switch(c)
		{
			case 'h':
				cout << "Usage: randsvm -n <no. of constraints> -d <no. of features> -s <sample size> ";
				cout << " -z <(c)lassification/(r)egression> -p <If regression, then size of epsilon tube> -t <solver type> -e <acceptable training error fraction> ";
				cout << "--bindir=<Path of SVM solver binary> --svmoptions=<Any options required for solver> ";
				cout << "inputfile modelfile [svs file] [weights file] [violators file]" << endl;
				cout << "-t - 0-SVMLight, 1-LibSVM, 2-LPSolve" << endl;
				cout << "The bindir should have files with the name \"learner\" and \"classifier\" which point to the appropriate binaries" << endl;
				exit(0);
				
			case 'z':
				t = optarg;
				if( t == "r" )
				{
					pt = regression;
					cout << "Solving regression problem." << endl;
				}
				else
				{
					cout << "Solving classification problem." << endl;
				}
				
				break;
			case 'p':
				istringstream(optarg) >> eps;
				cout << "Size of epsilon tube = " << eps << endl;
				break;
			case 'e':
				istringstream(optarg) >> err;
				cout << "Training error acceptable = " << err << endl;
				break;
			case 'd':
				istringstream(optarg) >> no_of_variables;
				cout << "Number of variables = " << no_of_variables << endl;
				break;
			case 'n':
				istringstream(optarg) >> no_of_constr;
				cout << "Number of constraints = " << no_of_constr << endl;
				break;
			case 's':
				istringstream(optarg) >> sample_size;
				cout << "Sample Size = " << sample_size << endl;
				break;
			case 'm': 
				istringstream(optarg) >> memory_size;
				cout << "Memory Size = " << memory_size << "MB" << endl;
				break;
			case 't':
				istringstream(optarg) >> solver_name;
				if (solver_name == LIBSVM )
					cout << "Using LIBSVM as the solver" << endl;
				else if( solver_name == SVMLight )
				{
					cout << "Using SVMLight as the solver" << endl;
					solver_name = SVMLight;
				}
				else
				{
					cout << "Using LPSolve as the solver" << endl;
					solver_name = LPSolve;
				}
				break;
			case 1:
				bindir = optarg;
				cout << "Bindir = " << bindir << endl;
				break;
			case 2:
				svm_options = optarg;
				cout << "SVM options = " << svm_options << endl;
				break;
		}
	}
	if( pt == regression && eps == -1 )
	{
		cerr << "Size of the epsilon tube not specified. " << endl;
		exit(1);
	}
	if( optind < argc )
	{
		ifile = argv[optind++];
		if( optind < argc )
			mfile = argv[optind++];
		cout << "Input file = " << ifile << endl;
		cout << "Model file = " << mfile << endl;
		
#ifdef DEBUG
		if( optind < argc )
			sv_file = argv[optind++];
		if( optind < argc )
			weights_file = argv[optind++];
		if( optind < argc )
			violators_file = argv[optind++];
		cout << "SVs file = " << sv_file << endl;
		cout << "Weights file = " << weights_file << endl;
		cout << "Violators file = " << violators_file << endl;
#endif
	}
	
#ifdef DEBUG
	Solve(no_of_variables, no_of_constr, sample_size, bindir, svm_options, ifile, mfile, memory_size, err, solver_name, sv_file, weights_file, violators_file, pt, eps);
#else
	Solve(no_of_variables, no_of_constr, sample_size, bindir, svm_options, ifile, mfile, memory_size, err, solver_name, pt, eps);
#endif

	return EXIT_SUCCESS;
}

#ifdef DEBUG
void Solve(const ulong no_of_variables, const size_t no_of_constr,const size_t sample_size, const string bindir, const string learner_options, const string ifile,const string mfile, const size_t memory_size, const double err, const int solver_name, const string sv_file, const string weights_file, const string violators_file, problem_type pt, double eps )
#else
void Solve(const ulong no_of_variables, const size_t no_of_constr,const size_t sample_size, const string bindir, const string learner_options, const string ifile,const string mfile, const size_t memory_size, const double err, const int solver_name, problem_type pt, double eps)
#endif
{
	string svm_learner =  bindir + "/learner";
	string svm_classifier = bindir + "/classifier";
	string ofile("/tmp/RandSVM_OP_File");
	
	SAMPLER s(no_of_variables, no_of_constr, SAMPLE_SIZE_MULTIPLIER*sample_size);
	
	
	
	if( solver_name == SVMLight )
	{
#ifdef DEBUG
		if( pt == classification )
		{
			RND_SVMLight solver(ifile, ofile, mfile, svm_learner, learner_options, svm_classifier, no_of_variables, no_of_constr, sample_size, err, memory_size, s, sv_file, weights_file, violators_file);
			solver.LearnModel();
		}
		else
		{
			RND_SVMLight solver(ifile, ofile, mfile, svm_learner, learner_options, svm_classifier, no_of_variables, no_of_constr, sample_size, err, memory_size, s, sv_file, weights_file, violators_file, pt, eps);
			solver.LearnModel();
		}
#else
		if( pt == classification )
		{
			RND_SVMLight solver(ifile, ofile, mfile, svm_learner, learner_options, svm_classifier, no_of_variables, no_of_constr, sample_size, err, memory_size, s);
			solver.LearnModel();
		}
		else
		{
		// VINAY MODIFICATION - UMIMPLEMENTED REGRESSION CODE 	
			RND_SVMLight solver(ifile, ofile, mfile, svm_learner, learner_options, svm_classifier, no_of_variables, no_of_constr, sample_size, err, memory_size, s, pt, eps);
			solver.LearnModel();
		}
#endif
	}
	else if( solver_name == LIBSVM )
	{
		RND_LIBSVM solver(ifile, ofile, mfile, svm_learner, learner_options, svm_classifier, no_of_variables, no_of_constr, sample_size, err, memory_size, s);
		solver.LearnModel();
	}
	else if( solver_name == LPSolve )
	{
		/* ITERSAMPLP solver(ifile, ofile, mfile, svm_learner, learner_options, svm_classifier, no_of_variables, no_of_constr, sample_size, err, memory_size, s);
		solver.LearnModel(); */ 
	}
		
	struct rusage self, children;
	getrusage(RUSAGE_SELF, &self);
	getrusage(RUSAGE_CHILDREN, &children);
	
	cout << "Total time taken = " << self.ru_utime.tv_sec + self.ru_stime.tv_sec + children.ru_utime.tv_sec  + children.ru_stime.tv_sec << endl;
}
