/*
 * big_to_little_endian.c
 *
 * Created: 22-10-2014 17:11:15
 *  Author: Kenneth
 */ 

#include "big_to_little_endian.h"
#include "../accident_data.h"

//#define SWAP_UINT16(x) (((x) >> 8) | ((x) << 8))
//#define SWAP_INT32(x) (((((x) >> 24) | (((x) & 0x00FF0000) >> 8) | (((x) & 0x0000FF00) << 8) | ((x) << 24))) & 0xFFFF)
//#define SWAP_UINT32(x) (((x) >> 24) | (((x) & 0x00FF0000) >> 8) | (((x) & 0x0000FF00) << 8) | ((x) << 24))

unsigned char _msd_file_stream[120] ; 

//! Byte swap unsigned short
uint16_t swap_uint16( uint16_t val )
{
	return (val << 8) | (val >> 8 );
}

//! Byte swap short
int16_t swap_int16( int16_t val )
{
	return (val << 8) | ((val >> 8) & 0xFF);
}

//! Byte swap unsigned int
uint32_t swap_uint32( uint32_t val )
{
	val = ((val << 8) & 0xFF00FF00 ) | ((val >> 8) & 0xFF00FF );
	return (val << 16) | (val >> 16);
}

//! Byte swap int
int32_t swap_int32( int32_t val )
{
	val = ((val << 8) & 0xFF00FF00) | ((val >> 8) & 0xFF00FF );
	return (val << 16) | ((val >> 16) & 0xFFFF);
}

uint64_t swap_uint64( uint64_t val )
{
	val = ((val << 8) & 0xFF00FF00FF00FF00ULL ) | ((val >> 8) & 0x00FF00FF00FF00FFULL );
	val = ((val << 16) & 0xFFFF0000FFFF0000ULL ) | ((val >> 16) & 0x0000FFFF0000FFFFULL );
	return (val << 32) | (val >> 32);
}

int64_t swap_int64( int64_t val )
{
	val = ((val << 8) & 0xFF00FF00FF00FF00ULL ) | ((val >> 8) & 0x00FF00FF00FF00FFULL );
	val = ((val << 16) & 0xFFFF0000FFFF0000ULL ) | ((val >> 16) & 0x0000FFFF0000FFFFULL );
	return (val << 32) | ((val >> 32) & 0xFFFFFFFFULL);
}

	
void convert_file_stream()
{
	uint8_t i = 0;

	/* CONTROL */
	_msd_file_stream[0] = _msd.control;
	
	/* VIN */
	for (i = 0; i < 20; i++)
	{
		_msd_file_stream[(i+1)] = _msd.VIN[i];
	}
	
	/* time_stamp */
	_msd_file_stream[21] = _msd.time_stamp >> 24;
	_msd_file_stream[22] = _msd.time_stamp >> 16;
	_msd_file_stream[23] = _msd.time_stamp >> 8;
	_msd_file_stream[24] = _msd.time_stamp;
	
	/* latitude */
	_msd_file_stream[25] = (_msd.latitude >> 24) & 0xFF;
	_msd_file_stream[26] = (_msd.latitude >> 16) & 0xFF;
	_msd_file_stream[27] = (_msd.latitude >> 8) & 0xFF;
	_msd_file_stream[28] = _msd.latitude & 0xFF;
	
	/* longitude */
	_msd_file_stream[29] = (_msd.longitude >> 24) & 0xFF;
	_msd_file_stream[30] = (_msd.longitude >> 16) & 0xFF;
	_msd_file_stream[31] = (_msd.longitude >> 8) & 0xFF;
	_msd_file_stream[32] = _msd.longitude & 0xFF ;
	
	_msd_file_stream[33] = _msd.direction;
	
	_msd_file_stream[34] = _msd.sp[0];
	_msd_file_stream[35] = _msd.sp[1];
	_msd_file_stream[36] = _msd.sp[2]; 
	_msd_file_stream[37] = _msd.sp[3];
	
	for (i = 0; i < 120; i++)
	{
		_msd_file_stream[38+i] = _msd.optional_data[i];
	}
}
