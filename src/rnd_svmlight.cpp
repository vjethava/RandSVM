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
#include "rnd_svmlight.h"
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstring>
#include <math.h>

RND_SVMLight::RND_SVMLight(const string input_file, const string output_file, const string model_file, const string learner_cmd, 
			const string learner_opts, const string classifier_cmd, const ulong no_of_variables,  const size_t no_of_constraints, 
			const size_t ss, const double err, const size_t memory_size, SAMPLER &s, const string svfile, const string wfile, 
			const string vfile, problem_type t, double ep): 
			RND_SVM(input_file, no_of_variables, no_of_constraints, ss, memory_size, err, s, svfile, wfile, vfile), 
			mfile(model_file), ofile(output_file), 
			learner_command(learner_cmd), learner_options(learner_opts), classifier_command(classifier_cmd), type(t), eps(ep) {
    pid_t pid = getpid();
    ostringstream pid_str;
    pid_str << pid;

    afile = "/tmp/Alphas." + pid_str.str();

    //cout << "Sample size in RND_SVMLIGHT constructor = " << sample_size << endl;
    alphas = new double[sample_size];

    InitializeLearnerArgs();
    InitializeClassifierArgs();
}

void RND_SVMLight::InitializeLearnerArgs() {
    vector<string> learner_opts;
    unsigned l = learner_options.length();
    unsigned indx = 0, start = 0;
    //Ignore the initial white space characters.
    for(; indx < l && (learner_options[indx] == ' '
                       || learner_options[indx] == '\t') ; indx++ )
        ;

    start = indx;

    //Extract the substrings separated by space
    for( ; indx < l; indx++ ) {
        if( learner_options[indx] == ' ' ) {
            learner_opts.push_back( learner_options.substr(start, indx-start) );
            start = indx;
            for(; indx < l && learner_options[indx] == ' ' ; indx++ )
                ;
            start = indx;
        }
    }
    if( start < l && learner_options[start] != ' ' )
        learner_opts.push_back( learner_options.substr(start, indx-start) );
    if( !use_all_avail_mem )
        learner_argv = new char *[learner_opts.size() + 8];
    else
        learner_argv = new char *[learner_opts.size() + 6];
    learner_argv[0] = "svm_learn";
    unsigned i = 0;
    for( ; i < learner_opts.size() ; i++ ) {
        learner_argv[i+1] = new char [learner_opts[i].length() + 1];
        strcpy(learner_argv[i+1], learner_opts[i].c_str() );
    }

    if( !use_all_avail_mem ) {
        i++;
        ostringstream os;
        os << memory_size;
        learner_argv[i++] = "-m";
        learner_argv[i] = new char [os.str().size() + 1];
        strcpy(learner_argv[i], os.str().c_str() );
    }

    learner_argv[i+1] = "-a";
    learner_argv[i+2] = new char[ afile.length() + 1 ];
    strcpy(learner_argv[i+2], afile.c_str() );

    learner_argv[i+3] = new char[ sfile.length() + 1 ];
    strcpy(learner_argv[i+3], sfile.c_str());

    learner_argv[i+4] = new char[ mfile.length() + 1 ];
    strcpy(learner_argv[i+4], mfile.c_str()) ;

    learner_argv[i+5] = 0;
}

void RND_SVMLight::InitializeClassifierArgs() {
    classifier_argv = new char *[7];
    classifier_argv[0] = "svm_classify";
    //Change the following later
    classifier_argv[1] = "-v";
    classifier_argv[2] = "3";



    //classifier_argv[3] = new char[ ifile.length() + 1 ];
    classifier_argv[3] = new char[ classifier_input_file.length() + 1 ];
    //strcpy(classifier_argv[3], ifile.c_str());
    strcpy(classifier_argv[3], classifier_input_file.c_str());

    classifier_argv[4] = new char[ mfile.length() + 1 ];
    strcpy(classifier_argv[4], mfile.c_str()) ;

    classifier_argv[5] = new char[ ofile.length() + 1 ];
    strcpy(classifier_argv[5], ofile.c_str() );

    classifier_argv[6] = 0;
}

void RND_SVMLight::Solve() {
    pid_t cpid;
    cout<<"Learner Command: "<<learner_command<<" ";
    for(int i=0; learner_argv[i] != '\0'; i++) {
        cout<<" "<<learner_argv[i];
    }
    cout<<endl;

    solver_failed = false;

    if( (cpid = fork() ) == 0 ) {
        if( execv( learner_command.c_str(), learner_argv ) )
            perror("RND_SVMLight::Solve");
        return;
    } else if( cpid == -1 ) {
        perror("RND_SVMLight::Solve");
        solver_failed = true;
        return;
    }

    int status;
    wait( &status );

    if( !WIFEXITED(status) ) {
        cerr << "RND_SVMLight::Solve : svm learner exited abnormally" << endl;
        solver_failed = true;
        return;
    }
}

size_t RND_SVMLight::FindViolators(const bool *sampled_constr) {
    //Part A
    SAMPLER s(no_of_variables, no_of_constraints, 50000);
    const bool *sampled_classifier_examples;
    sampled_classifier_examples = s.ChooseSample(NULL);

    ifstream in1(ifile.c_str() );
    if( !in1.good() ) {
        cerr << "Unable to open input file for reading" << endl;
        return 0;
    }

    ofstream clout(classifier_input_file.c_str() );
    if( !clout.good() ) {
        cerr << "Unable to open sample file for writing" << endl;
        return 0;
    }

    string line;
    for( ulong i = 0; i < no_of_constraints; i++) {
        getline(in1, line);
        if( sampled_classifier_examples[i] )
            clout << line << endl;
    }


    clout.close();
    in1.close();
    //Part 2
    pid_t cpid;


    if( (cpid = fork() ) == 0 ) {
        if( execv( classifier_command.c_str(), classifier_argv ) )
            perror("RND_SVMLight::FindViolators");
        return 0;
    } else if( cpid == -1 ) {
        perror("RND_SVMLight::FindViolators");
        return 0;
    }

    int status;
    wait( &status );

    if( !WIFEXITED(status) ) {
        cerr << "RND_SVMLight::FindViolators : svm classifier exited abnormally" << endl;
        return 0;
    }



    ifstream ain( afile.c_str() );
    if( !ain.good() ) {
        cerr << "RND_SVMLight::FindViolators : Unable to open file " << afile << endl;
        return 0;
    }

    cout << "Checking for violators" << endl;
    if( !alphas ) {
        cerr << "RND_SVMLight::FindViolators : Unable to allocate memory for array alphas" << endl;
        return 0;
    }

    cout << "Initializing alpha array" << endl;
    for( size_t i = 0; i < sample_size; i++ ) {
        ain >> alphas[i];
    }
    ain.close();
    cout << "Array initialization done" << endl;

    ifstream in( ofile.c_str() );

    if( !in.good() ) {
        cerr << "RND_SVMLight::FindViolators : Unable to open file " << ofile << endl;
        return 0;
    }

    double eps;
    if( type == regression ) {
        //Extract the eps value from the model file
    }
    size_t no_of_violators = 0;
    size_t sampled_constr_ctr = 0;
    no_of_sv = 0;
    cout << "Checking for violations" << endl;

    for( ulong i = 0; i < no_of_constraints; i++ ) {
        violators[i] = false;
        sv[i] = false;

        string a, b,category_str;
        //in >> a >> b;



        //if( !sampled_constr[i] )
        //{
        //	string::size_type pos = a.find(":", 0);
        //	string category_str = a.substr(pos+1, 2);
        double fx;//f(x) = w.x + b
        int category;
        //istringstream(category_str) >> category;
        if( sampled_classifier_examples[i] )
            in >> fx;

        //istringstream(a) >> category;
        /*if( category > 0 )
        	category = true;
        else
        	category = false;*/
        category = fx > 0 ? 1 : -1;

        bool is_sv = false;
        if( sampled_constr[i] )
            if( sampled_constr_ctr == sample_size ) {
                cout << "Woopsy!" << endl;
                return 0;
            } else
                is_sv = alphas[sampled_constr_ctr++];

        if( is_sv ) {
            sv[i] = true;
            no_of_sv++;
            continue;
        }

        if( type == classification ) {
            if( sampled_classifier_examples[i] && (category != categories[i] || fabs(fx) < (1-0.001)) ) {

                violators[i] = true;
                if( !sampled_constr[i] )
                    no_of_violators++;

            }
        } else //regression {
            if( sampled_classifier_examples[i] && ( fabs(fx-categories[i]) > eps+0.001 ) ) {
                violators[i] = true;
                if( !sampled_constr[i] )
                    no_of_violators++;
            }
    }
    //}}
cout << "Violators check done" << endl;
return no_of_violators;
}

RND_SVMLight::~RND_SVMLight() {
    unlink(ofile.c_str());
    unlink(afile.c_str());
}


