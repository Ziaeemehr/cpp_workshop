#ifndef TIME_H
#define TIME_H 

class Time 
{
    private :
        int hour;
        int minute;
        int second;
    public:
        Time(const int h=0, const int m=0, const int s=0);
        void setTime(const int h, const int m, const int s);
        void print() const;
        bool equals(const Time&);
};

#endif
