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

#endif
