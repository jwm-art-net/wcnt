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
    const char* const fxstring; /* for fixed string param type */
    wave_tables();
    ~wave_tables();
    void calc_phase_step_scalar(unsigned long samplerate);
    const double* const get_table(WT);
    static const int table_bits;
    static const int table_shift;
    static double phase_step_base;
    static double phase_step_scalar;
  private:
    unsigned long table_size;
    double* table[WT_LAST];
    void create_table(WT);
};

#endif
