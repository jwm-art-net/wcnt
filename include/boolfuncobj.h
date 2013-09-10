#ifndef BOOLFUNCOBJ_H
#define BOOLFUNCOBJ_H

class boolean
{
 public:
    boolean(bool & n) : _bool(n) {};
    boolean(bool const & n) : _bool(n) {};

    bool operator()(bool & n) const {
        return _bool == n;
    }

    bool operator()(const bool & n) const {
        return _bool == n;
    }

 private:
    bool _bool;
};

class valid
{
 public:
    valid(bool & n) : _bool(n) {};
    valid(bool const & n) : _bool(n) {};

    bool operator()(bool & n) const {
        return _bool == n;
    }

    bool operator()(const bool & n) const {
        return _bool == n;
    }

 private:
    bool _bool;
};

class empty_run
{
 public:
    empty_run(bool & n) : _bool(n) {};
    empty_run(bool const & n) : _bool(n) {};

    bool operator()(bool & n) const {
        return _bool == n;
    }

    bool operator()(const bool & n) const {
        return _bool == n;
    }

 private:
    bool _bool;
};

#endif
