
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

#ifndef __Raster_hxx__
#define __Raster_hxx__

#include <Point2D.hxx>
#include <StaticRaster.hxx>

namespace Engine
{

//! Raster adds mechanisms to modify the values of the raster map. It is serialized each time step.
class Raster : public StaticRaster
{
	std::vector< std::vector<int> >	_maxValues;
	int				_currentMaxValue;
	int				_currentMinValue;
public:
	Raster();
	virtual ~Raster();

	// parameters: starting pos and size in matrix to grow
	//! Increases each cell value by 1 if it is under the maximum allowed.
	void updateRasterIncrement();
	// parameters: starting pos and size in matrix to grow
	//! Assigns to each cell in raster the max value allowed for it.
	void updateRasterToMaxValues();
	
	//! Reads the maximum allowed value in the cell located by parameter "position". Returns -1 if "position" is out of the area of the raster.
	int getMaxValueAt( Point2D<int> position );
	
	//! Assigns the value "value" to the cell located by parameter "position". Does nothing if "position" is out of the area of the raster.
	virtual void setValue( Point2D<int> position, int value );
	//! Changes the maximum value allowed in the cell located by parameter "position" to the new amount "value". Does nothing if "position" is out of the area of the raster.
	void setMaxValue( Point2D<int> position, int value );
	
	//! Initializes the components of vector '_values' to defaultValue, and to maxValue the ones from vector _maxValue.
	void setInitValues( int minValue, int maxValue, int defaultValue );
	//! Sets new value for attribute maxValue.
	void setMaxValue( const int & maxValue);
	//! Sets new value for attribute minValue.
	void setMinValue( const int & minValue);
	void resize( const Point2D<int> & size );
	
	void updateCurrentMinMaxValues();

	int  getCurrentMinValue() const { return _currentMinValue; }
	int  getCurrentMaxValue() const { return _currentMaxValue; }

	friend class RasterLoader;
};

} // namespace Engine

#endif // __Raster_hxx__

