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
#include "sampler.h"
#include <iostream>

using namespace std;

SAMPLER::SAMPLER(const ulong &n,const size_t &d,const size_t &s): rndgen(d, 1.0/scale), rndgen1(d, 1.0/scale), no_of_variables(n), no_of_constraints(d), sample_size(s), total_weight(0)
{
	sampled_constr = new bool[no_of_constraints];
	prev_weights = new double[no_of_constraints];
	for( ulong i = 0; i < no_of_constraints; i++ )
		prev_weights[i] = 1.0/scale;
	if( !sampled_constr )
	{
		cerr << "Sampler::Sampler : Memory allocation failed" << endl;
		return;
	}
	total_weight = no_of_constraints/scale;
}

const bool * SAMPLER::ChooseSample(const bool *sv)
{
	for( size_t i = 0; i < no_of_constraints; i++ )
		sampled_constr[i] = false;

	rndgen.UpdateGenerator();
	rndgen1.UpdateGenerator();
	size_t i = 0;
	size_t total_generated = 0;
	
	ulong number_of_unique_samples = 0;		
	cout << "Picking samples" << endl;
	if( sv )
		for(ulong i = 0; i < no_of_constraints; i++ )
			if( sv[i] )
			{
				sampled_constr[i] = true;
				number_of_unique_samples++;
				total_generated++;
			}
	while( total_generated < sample_size )
	{
		size_t sample = rndgen.Get();
		total_generated++;
		
		if( !sampled_constr[sample] )
			number_of_unique_samples++;
			
		sampled_constr[sample] = true;
	}
	
	cout << "No. of unique examples sampled = " << number_of_unique_samples << endl;
	return sampled_constr;
}

const bool *SAMPLER::ChooseSample(const bool *violators, const bool *sv)
{
	ulong no_sampled = 0;
	ulong no_unique = 0;
	rndgen.UpdateGenerator();
	rndgen1.UpdateGenerator();
	
	
	for( unsigned i = 0; i < no_of_constraints ; i++ )
	{
		sampled_constr[i] = false;
		if( violators[i] || sv[i] )
		{
			no_sampled++;
			no_unique++;
			sampled_constr[i] = true;
		}
	}
	
	//size_t total_generated = 0;
	while( no_sampled < sample_size )
	{
		size_t sample = rndgen1.Get();
		//total_generated++;
		
		if( !sampled_constr[sample] )
			no_unique++;
		
		sampled_constr[sample] = true;
		no_sampled++;
		
		
	}
	
	cout << "No. of unique examples sampled = " << no_unique << endl;
	return sampled_constr;
	
}

void SAMPLER::UpdateWeights(const bool *violators)
{
	total_weight = 0;
	for(ulong i = 0; i < no_of_constraints; i++ )
	{
		//double weight = rndgen.Weight(i);
		double weight = prev_weights[i];
		if( violators[i] )
		{
			rndgen.UpdateWeight(i, 2*weight);
			rndgen1.UpdateWeight(i, 2*weight);
			prev_weights[i] = 2*weight;
			total_weight += 2*weight;
			
		}
		else
		{
			//rndgen.UpdateWeight(i, weight);
			//total_weight += weight;
			rndgen.UpdateWeight(i, 0);
		}
	}
}

void SAMPLER::GetWeights(double *weights) const
{
	for( ulong i = 0; i < no_of_constraints; i++ )
		weights[i] = rndgen.Weight(i);
}

ldouble SAMPLER::TotalWeight() const
{
	return total_weight * scale;
}

ldouble SAMPLER::ViolatorsWeight(const bool *violators) const
{
	ldouble total = 0.0 ;
	for( size_t i = 0; i < no_of_constraints; i++ )
		if( violators[i] )
			total += rndgen.Weight(i);

	return total*scale;
}

bool SAMPLER::DoUpdateWeights(const bool *violators) const
{
	//ldouble total_weight_of_violators = ViolatorsWeight(violators);

	/*ldouble rhs = 2 * ( scale*total_weight - sample_size );
	rhs /= sample_size;
	rhs *= no_of_variables;

	if( total_weight_of_violators < rhs )*/
		return true;
	//return false;
}

void SAMPLER::SetSampleSize(size_t ss)
{
	sample_size = ss;
}

SAMPLER::~SAMPLER()
{
	if( sampled_constr )
		delete [] sampled_constr;
}
