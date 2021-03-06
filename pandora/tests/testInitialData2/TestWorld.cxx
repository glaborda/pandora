/*
 * Copyright (c) 2012
 * COMPUTER APPLICATIONS IN SCIENCE & ENGINEERING
 * BARCELONA SUPERCOMPUTING CENTRE - CENTRO NACIONAL DE SUPERCOMPUTACIÓN
 * http://www.bsc.es

 * This file is part of Pandora Library. This library is free software; 
 * you can redistribute it and/or modify it under the terms of the
 * GNU General Public License as published by the Free Software Foundation;
 * either version 3.0 of the License, or (at your option) any later version.
 * 
 * Pandora is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public 
 * License along with this library.  If not, see <http://www.gnu.org/licenses/>.
 * 
 */

#include "TestWorld.hxx"

#include "Raster.hxx"
#include "Point2D.hxx"
#include "Exceptions.hxx"

#include <assert.h>
#include <iostream>

namespace Test
{

TestWorld::TestWorld( const Engine::Simulation & sim ) : World(sim, 4, true, "data/test.h5")
{
}

TestWorld::~TestWorld()
{
}

void TestWorld::stepRasters()
{
	Engine::Point2D<int> index(0,0);
	for(index._x=_boundaries._origin._x; index._x<_boundaries._origin._x+_boundaries._size._x; index._x++)		
	{
		for(index._y=_boundaries._origin._y; index._y<_boundaries._origin._y+_boundaries._size._y; index._y++)			
		{
			assert(getValue("testX", index)==index._x);
			assert(getValue("testY", index)==index._y);
		}
	}
}

void TestWorld::createRasters()
{	
	registerDynamicRaster("testX", false);
	registerDynamicRaster("testY", false);
	getDynamicRaster("testX").setInitValues(0,_globalBoundaries._size._x, 0);
	getDynamicRaster("testY").setInitValues(0,_globalBoundaries._size._y, 0);

	Engine::Point2D<int> index(0,0);
	for(index._x=_boundaries._origin._x; index._x<_boundaries._origin._x+_boundaries._size._x; index._x++)		
	{
		for(index._y=_boundaries._origin._y; index._y<_boundaries._origin._y+_boundaries._size._y; index._y++)			
		{
			setMaxValue("testX", index, index._x);
			setMaxValue("testY", index, index._y);
		}
	}
	updateRasterToMaxValues("testX");
	updateRasterToMaxValues("testY");
}

void TestWorld::createAgents()
{
}

} // namespace Test 

