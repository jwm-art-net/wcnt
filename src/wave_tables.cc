#ifndef WAVE_TABLES_H
#include "../include/wave_tables.h"

#include <math.h>

wave_tables::wave_tables() :
  fxstring("one/sine/tri/saw/sine_01/tri_01/saw_01"),
  table_size(0)
{
    for(int i = 0; i < WT_LAST; i++)
        table[i] = 0;
    table_size = (1 << wave_tables::table_bits);
    if (phase_step_base == 0)
        phase_step_base = pow(2, sizeof(unsigned long) * 8) / 360;
}

wave_tables::~wave_tables()
{
    for (int i = 0; i < WT_LAST; i++)
        if (table[i])
            delete [] table[i];
}

void wave_tables::calc_phase_step_scalar(unsigned long samplerate)
{
    phase_step_scalar = (phase_step_base * 360) / samplerate;
}

const double* const wave_tables::get_table(WT type)
{
    if (!table[type])
        create_table(type);
    return table[type];
}

void wave_tables::create_table(WT type)
{
    unsigned long i;
    switch(type)
    {
        case ONE:
        {
            table[ONE] = new double[table_size];
            for (i = 0; i < table_size; i++)
                table[ONE][i] = 1.0;
            break;
        }
        case SINE:
        case SINE_01:
        {
            table[type] = new double[table_size];
            double shift = ((double)M_PI * 2.0) / table_size;
            for (i = 0; i < table_size; i++)
                table[type][i] = (type == SINE)
                    ? sin(shift * i)
                    : (1.0 + sin(shift * i)) / 2.0;
            break;
        }
        case TRI:
        case TRI_01:
        {
            table[type] = new double[table_size];
            int s;
            double lvl, dir;
            unsigned long ssz = table_size / 4;
            double rt = 1.0 / (double)ssz;
            for (s = 0; s < 4; s++){
                switch(s){
                    case 1: lvl = 1;  dir = -1; break;
                    case 2: lvl = 0;  dir = -1; break;
                    case 3: lvl = -1; dir = 1;  break;
                    default:
                        lvl = 0;  dir = 1;
                }
                for (i = 0; i < ssz; i ++){
                    table[type][s * ssz + i] = (type == TRI)
                        ? lvl 
                        : (1.0 + lvl) / 2.0;
                    lvl += rt * dir;
                }
            }
            break;
        }
        case SAW:
        case SAW_01:
        {
            table[type] = new double[table_size];
            double ht = table_size / 2;
            double rt = 1.0 / ht;
            for (i = 0; i < table_size; i++) {
                double l = -1 + ((unsigned long)
                    (i + ht) % table_size) * rt;
                table[type][i] = (type == SAW) ? l : (1.0 + l) / 2.0;
            }
            break;
        }
        default:
            return;
    }
}

const int wave_tables::table_bits = TABLE_BITS;
const int wave_tables::table_shift = TABLE_SHIFT;
double wave_tables::phase_step_base = 0;
double wave_tables::phase_step_scalar = 0;

#endif
