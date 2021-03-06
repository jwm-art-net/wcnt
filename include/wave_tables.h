#ifndef WAVE_TABLES_H
#define WAVE_TABLES_H

#define TABLE_BITS 14
#define TABLE_SHIFT (8 * sizeof(unsigned long) - TABLE_BITS)

class wave_tables
{
  public:
    enum WT
    {
        ONE,
        SINE,
        TRI,
        SAW,
        SINE_01,
        TRI_01,
        SAW_01,
        WT_LAST
    };

    static void instantiate() { static wave_tables making_waves; }

    static void calc_phase_step_scalar(unsigned long samplerate);
    static const double* get_table(WT);

    static const char* const fxstring; /* for fixed string param type */
    static const int table_bits;
    static const int table_shift;
    static double phase_step_base;
    static double phase_step_scalar;

  private:
    wave_tables();
    ~wave_tables();
    static unsigned long table_size;
    static double* table[WT_LAST];
    static void create_table(WT);
};

#endif
