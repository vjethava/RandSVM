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
#include "rnd_libsvm.h"
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstring>
#include <math.h>

RND_LIBSVM::RND_LIBSVM(const string input_file, const string output_file, const string model_file, const string learner_cmd, const string learner_opts, const string classifier_cmd, const ulong no_of_variables,  const size_t no_of_constraints, const size_t sample_size, const double err, const size_t memory_size, SAMPLER &s): RND_SVM(input_file, no_of_variables, no_of_constraints, sample_size, memory_size, err, s), mfile(model_file), ofile(output_file), learner_command(learner_cmd), learner_options(learner_opts), classifier_command(classifier_cmd)
{
	pid_t pid = getpid();
	ostringstream pid_str;
	pid_str << pid;
	
	sv_file = "/tmp/SV." + pid_str.str();
	
	//alphas = new double[sample_size];
	
	InitializeLearnerArgs();	
	InitializeClassifierArgs();
}

void RND_LIBSVM::InitializeLearnerArgs()
{
 	vector<string> learner_opts;
	unsigned l = learner_options.length();
	unsigned indx = 0, start = 0;
	for(; indx < l && learner_options[indx] == ' ' ; indx++ );
	
	start = indx;
	
	for( ; indx < l; indx++ )
	{
		if( learner_options[indx] == ' ' )
		{
			learner_opts.push_back( learner_options.substr(start, indx-start) );
			start = indx;
			for(; indx < l && learner_options[indx] == ' ' ; indx++ );
 			start = indx;
		}
	}
	if( learner_options[start] != ' ' )
  		learner_opts.push_back( learner_options.substr(start, indx-start) );
	if( !use_all_avail_mem )	
		learner_argv = new char *[learner_opts.size() + 8];
	else
		learner_argv = new char *[learner_opts.size() + 6];
	learner_argv[0] = "libsvm_learn";
	unsigned i = 0;
	for( ; i < learner_opts.size() ; i++ )
	{
		learner_argv[i+1] = new char [learner_opts[i].length() + 1];
		strcpy(learner_argv[i+1], learner_opts[i].c_str() );
	}
	
	if( !use_all_avail_mem )
	{
		i++;
		ostringstream os;
		os << memory_size;
		learner_argv[i++] = "-m";
		learner_argv[i] = new char [os.str().size() + 1];
		strcpy(learner_argv[i], os.str().c_str() );
	}
	
	/*learner_argv[i+1] = "-a";
	learner_argv[i+2] = new char[ afile.length() + 1 ];
	strcpy(learner_argv[i+2], afile.c_str() );*/
	
	learner_argv[i+1] = new char[ sfile.length() + 1 ];
	strcpy(learner_argv[i+1], sfile.c_str());
	
	learner_argv[i+2] = new char[ mfile.length() + 1 ];
	strcpy(learner_argv[i+2], mfile.c_str()) ;
	
	learner_argv[i+3] = new char[ sv_file.length() + 1 ];
	strcpy(learner_argv[i+3], sv_file.c_str());
	
	learner_argv[i+4] = 0;
}

void RND_LIBSVM::InitializeClassifierArgs()
{
	classifier_argv = new char *[7];
	classifier_argv[0] = "libsvm_classify";
	classifier_argv[1] = "-b";
	classifier_argv[2] = "0";
	
	
	classifier_argv[3] = new char[ classifier_input_file.length() + 1 ];
	strcpy(classifier_argv[3], classifier_input_file.c_str());
	
	classifier_argv[4] = new char[ mfile.length() + 1 ];
	strcpy(classifier_argv[4], mfile.c_str()) ;
	
	classifier_argv[5] = new char[ ofile.length() + 1 ];
	strcpy(classifier_argv[5], ofile.c_str() );
	
	classifier_argv[6] = 0;
}

void RND_LIBSVM::Solve()
{
  	pid_t cpid;
	
	solver_failed = false;

	if( (cpid = fork() ) == 0 )
	{
		if( execv( learner_command.c_str(), learner_argv ) )
			perror("RND_LIBSVM::Solve - execv()");	
		return;
	}	
	else if( cpid == -1 )
	{
		perror("RND_LIBSVM::Solve - fork()");
		solver_failed = true;
		return;
	}

	int status;
	wait( &status );

	if( !WIFEXITED(status) )
	{
		cerr << "RND_LIBSVM::Solve : svm learner exited abnormally" << endl;
		solver_failed = true;
		return;
	}
}

size_t RND_LIBSVM::FindViolators(const bool *sampled_constr) 
{
	pid_t cpid;
	
	//Part A	
	SAMPLER s(no_of_variables, no_of_constraints, 50000);
	const bool *sampled_classifier_examples;
	sampled_classifier_examples = s.ChooseSample(NULL);
	
 	ifstream in1(ifile.c_str() );
	if( !in1.good() )
	{
		cerr << "Unable to open input file for reading" << endl;
		return 0;
	}

	ofstream clout(classifier_input_file.c_str() );
	if( !clout.good() )
	{
		cerr << "Unable to open sample file for writing" << endl;
		return 0;
	}

	string line;
	for( ulong i = 0; i < no_of_constraints; i++) 
	{
		getline(in1, line);
		if( sampled_classifier_examples[i] )
			clout << line << endl;
	}
	

	clout.close();
	in1.close();
	//Part 2
	if( (cpid = fork() ) == 0 )
	{
		if( execv( classifier_command.c_str(), classifier_argv ) )
			perror("RND_LIBSVM::FindViolators - execv()");
		return 0;
	}
	else if( cpid == -1 )
	{
		perror("RND_LIBSVM::FindViolators - fork() ");
		return 0;
	}
	
	int status;
	wait( &status );

	if( !WIFEXITED(status) )
	{
		cerr << "RND_LIBSVM::FindViolators : svm classifier exited abnormally" << endl;
		return 0;
	}
	
	
	
	ifstream in(ofile.c_str());

  	if( !in.good() )
	{
		cerr << "RND_LIBSVM::FindViolators : Unable to open file " << ofile << endl;
		return 0;
	}	

	ifstream sv_fp( sv_file.c_str() );
	if( !sv_fp.good() )
	{
		cerr << "RND_LIBSVM::FindViolators : Unable to open file " << sv_file << endl;
		return 0;
	}
	ulong *sv_indices = new ulong[sample_size];
	no_of_sv = 0;
	for(unsigned i=0; !sv_fp.eof(); i++ )
	{
		string line;
		getline(sv_fp, line);
		istringstream(line) >> sv_indices[i];
		if( sv_fp.good() )
			no_of_sv++;
	}
	cout << "Number of Support Vectors read from SV file = " << no_of_sv << endl;
	sortSVIndices(sv_indices, no_of_sv);

	size_t no_of_violators = 0;
	size_t sampled_constr_ctr = 0;
	size_t sv_ctr = 0;
	no_of_sv = 0;
	cout << "Checking for violations" << endl;
	
	for( ulong i = 0; i < no_of_constraints; i++ )
	{
	
		violators[i] = false;
		sv[i] = false;
		
		int category;
		double fx;
		if( sampled_classifier_examples[i] )
			in >> fx;
					
		category = fx > 0 ? true : false;
		bool is_sv = false;
		if( sampled_constr[i] )
				if( sampled_constr_ctr == sample_size )
				{
					cout << "Woopsy!" << endl;
					return 0;
				}
				else
				{
					if( sampled_constr_ctr == sv_indices[sv_ctr] )
					{
						is_sv = true;
						sv_ctr++;
					}
					sampled_constr_ctr++;
				}
			
		if( is_sv )
		{
			sv[i] = true;
			no_of_sv++;
		}
			
			
		if( sampled_classifier_examples[i] && (category != categories[i] || fabs(fx) < (1-0.001)) )
 		{
			violators[i] = true;
			if( !sampled_constr[i] && sampled_classifier_examples[i])
				no_of_violators++;
		}
	}	
	cout << "Violators check done" << endl;
	return no_of_violators;
}

RND_LIBSVM::~RND_LIBSVM()
{
	unlink(ofile.c_str());
}
