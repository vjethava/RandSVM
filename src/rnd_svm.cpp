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
#include "rnd_svm.h"
#include <fstream>
#include <sstream>
#include <unistd.h>

#include <sys/resource.h>
#include <sys/time.h>
#include <unistd.h>

RND_SVM::RND_SVM(const string input_file, const ulong n, const size_t d, const size_t ss, const size_t mem_size, const double error, SAMPLER &sampler, const string svfile, const string wfile , const string vfile): ifile(input_file), no_of_variables(n), no_of_constraints(d), k(ss), sample_size(SAMPLE_SIZE_MULTIPLIER*ss), err(error), s(sampler), no_of_sv(0), no_of_violators(0), sv_file(svfile), weights_file(wfile), violators_file(vfile)
{
	pid_t pid = getpid();
	ostringstream pid_str;
	pid_str << pid;
	
	sfile = "./SampleFile." + pid_str.str(); 
	classifier_input_file = "./ClassifierInput." +  pid_str.str();
	
	violators = new bool[no_of_constraints];

	if( !violators )
	{
		cerr << "RND_SVM: Unable to allocate memory" << endl;
		return;
	}

	categories = new int[no_of_constraints];
	if( !categories )
	{
		cerr << "RND_SVM: Unable to allocate memory" << endl;
		delete []violators;
		return;
	}

	sv = new bool[no_of_constraints];
	if( !sv )
	{
		cerr << "RND_SVM: Unable to allocate memory" << endl;
		delete []categories;
		delete []violators;
		return;
	}
	
	ifstream in(ifile.c_str() );
	unsigned long pos_docs=0, neg_docs=0;
	for( unsigned i = 0; i < no_of_constraints; i++ )
	{
		string line;
		getline(in, line);
		int category;
		istringstream(line) >> category;
		
		categories[i] = category;
		if( category > 0 )
		{
			//categories[i] = true;
			pos_docs++;
		}
		else
		{
			//categories[i] = false;
			neg_docs++;
		}
	}
	cout << "No. of positive examples = " << pos_docs << endl << "No. of negative examples = " << neg_docs << endl;
	
	
	
	if( sv_file != "" )
		svout.open(sv_file.c_str());
	if( !svout.good() )
	{
		cout << " Damn!" << endl;
		exit(1);
	}
	
	if( violators_file != "" )
		vout.open(violators_file.c_str());
	if( weights_file != "" )
		wout.open(weights_file.c_str());
		
	if( !mem_size )
		use_all_avail_mem = true;
	else
		use_all_avail_mem = false;
	memory_size = mem_size;
}

void RND_SVM::sortSVIndices(ulong *sv_indices, ulong no_svs)
{
  for (ulong i=1; i < no_svs; i++)
  {
    ulong index = sv_indices[i];
    ulong j = i;
    while ((j > 0) && (sv_indices[j-1] > index))
    {
      sv_indices[j] = sv_indices[j-1];
      j = j - 1;
    }
    sv_indices[j] = index;
  }
}

void RND_SVM::print_timing(const string &message)
{
	struct rusage self, children;
	getrusage(RUSAGE_SELF, &self);
	//getrusage(RUSAGE_CHILDREN, &children);
	//cout << message << self.ru_utime.tv_sec + self.ru_stime.tv_sec + children.ru_utime.tv_sec  + children.ru_stime.tv_sec << " seconds" << endl;
	cout << message << self.ru_utime.tv_sec + self.ru_stime.tv_sec << " seconds " << endl;
}

void RND_SVM::LearnModel()
{
	bool sample_size_changed = false;
	const bool *v = violators;
	unsigned iteration = 1;
	do
	{
		cout << "Iteration " << iteration << endl;
		
		const bool *sampled_constr;
		if(iteration == 1 )
			sampled_constr = s.ChooseSample(NULL);
		else if( no_of_violators + no_of_sv > sample_size )
			sampled_constr = s.ChooseSample(sv);
		else
			sampled_constr = s.ChooseSample(violators, sv);
		
		//print_timing("Sampling done: ");
		
		
		WriteSampleFile(sampled_constr);
		
		//print_timing("Sample file written: ");
		
		Solve();
		
		//print_timing("Learning done: ");

	  	if( solver_failed )
		{
			cerr << "SVM Solver failed!" << endl;
			cerr << "Skipping to next iteration" << endl;
			continue;
		}	

		no_of_violators = FindViolators(sampled_constr);
		
		print_timing("Violators found: ");
		
		cout << "Number of Violators = " << no_of_violators << endl;
		cout << "Number of Support Vectors = " << no_of_sv << endl;

		if( svout.good() )
			DumpSV();
		
		if( vout.good() )
			DumpViolators();
		if( wout.good() )
			DumpWeights();
			
		double threshold = err * no_of_constraints;
		cout << "Threshold = " << threshold << endl;
		
		print_timing("Time taken till iteration(without including time by learner & classifier): ");
		
		if( no_of_violators <= threshold || no_of_sv >= k )
			break;

		
		if( s.DoUpdateWeights(v) )
		{
			s.UpdateWeights(v);
			cout << "Weights updated" << endl;
		}
		else
		{
			cout << "Weights not updated" << endl;
		}
		
			/*if( no_of_violators <= .5/100 * no_of_constraints && !sample_size_changed )
		{
			sample_size *= 1.1;
			s.SetSampleSize(sample_size);
			cout << "Sample size changed to " << sample_size << endl;
			sample_size_changed = true;
		}*/
		
		print_timing("Iteration done: ");
		iteration++;
	}while(1);
}

void RND_SVM::WriteSampleFile(const bool *sampled_constr)
{
	ifstream in(ifile.c_str() );
	if( !in.good() )
	{
		cerr << "Unable to open input file for reading" << endl;
		return;
	}

	ofstream sout(sfile.c_str() );
	if( !sout.good() )
	{
		cerr << "Unable to open sample file for writing" << endl;
		return;
	}

	string line;
	size_t sampled_constr_ctr = 0;
	for( ulong i = 0; i < no_of_constraints; i++) 
	{
		getline(in, line);
		if( sampled_constr[i] )
			sout << line << endl;
	}
	

	sout.close();
	in.close();
}

void RND_SVM::DumpSV()
{
	svout << sv[0];
	for( ulong i = 1; i < no_of_constraints; i++ )
		svout << " " << sv[i];
	svout << endl;
	
}

void RND_SVM::DumpViolators()
{
	vout << violators[0];
	for( ulong i = 1; i < no_of_constraints; i++ )
		vout << " " << violators[i];
	vout << endl;
}

void RND_SVM::DumpWeights()
{
	double *weights = new double[no_of_constraints];
	s.GetWeights(weights);
	wout << weights[0];
	for( ulong i = 1; i < no_of_constraints; i++ )
		wout << " " << weights[i];
	wout << endl;
}

RND_SVM::~RND_SVM()
{
	if( violators )
	{
		delete [] violators;
		delete [] categories;
		
	}
	//unlink(sfile.c_str());
	unlink(classifier_input_file.c_str());
}


