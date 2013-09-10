#ifndef MISCFUNCOBJ_H
#define MISCFUNCOBJ_H

class _pos_
{
 public:
    _pos_(double & n) : value(n) {};
    _pos_(double const & n) : value(n) {};

    bool operator()(double & n) const {
        return value == n;
    }

    bool operator()(const double & n) const {
        return value == n;
    }
 private:
    const double & value;
};

class _len_
{
 public:
    _len_(double & n) : value(n) {};
    _len_(double const & n) : value(n) {};

    bool operator()(double & n) const {
        return value == n;
    }

    bool operator()(const double & n) const {
        return value == n;
    }
 private:
    const double & value;
};

class _vel_
{
 public:
    _vel_(double & n) : value(n) {};
    _vel_(double const & n) : value(n) {};

    bool operator()(double & n) const {
        return value == n;
    }

    bool operator()(const double & n) const {
        return value == n;
    }
 private:
    const double & value;
};


class _start_bar_
{
 public:
    _start_bar_(short & n) : value(n) {};
    _start_bar_(short const & n) : value(n) {};

    bool operator()(short & n) const {
        return value == n;
    }

    bool operator()(short const & n) const {
        return value == n;
    }
 private:
    const short & value;
};

#endif
