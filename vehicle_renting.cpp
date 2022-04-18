#include <iostream>
#include <iomanip>
#include <cstdlib>

using namespace std;

class DayOfYear
{
private:
    unsigned short int dayOfYear, year;
    static const int days[12];

public:
    DayOfYear(int d = 1, int y = 2021):
        dayOfYear(d),
        year(y)
    {}

    DayOfYear(int y, int m, int d)
    {
        dayOfYear = days[m - 1] + d;
        year = y;
    }

    unsigned short int get_dayOfYear()
    {
        return dayOfYear;
    }

    friend DayOfYear operator++(DayOfYear &d)
    {
        if(d.dayOfYear < 365)
        {
            d.dayOfYear++;
        }
        else
        {
            d.dayOfYear = 1;
            d.year++;
        }
        return d;
    }

    friend istream& operator>>(istream& in, DayOfYear& d)
    {
        char c = '-';
        int mm, dd;
        in >> d.year >> c >> mm >> c >> dd;
        d.dayOfYear = days[mm - 1] + dd;
        return in;
    }

    friend ostream& operator<<(ostream& out, const DayOfYear& d)
    {
        char c = '-';
        int i = 0;
        while(i < 12)
        {
            if(d.dayOfYear <= days[i])
            {
                return out << setfill('0') << setw(4) << d.year << c
                           << setfill('0') << setw(2) << i << c
                           << setfill('0') << setw(2) << d.dayOfYear - d.days[i - 1];
            }
            i++;
        }
    }
};

const int DayOfYear::days[12] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};

class Vehicle
{
private:
    const int no;
    string model;
    float price24h;

public:
    Vehicle(int x, string s, float f):
        no(x),
        model(s),
        price24h(f)
    {}

    virtual ~Vehicle()
    {
        cout << "Vehicle destructor" << endl;
    }

    int get_no()
    {
        return no;
    }

    string get_model()
    {
        return model;
    }

    float get_price(int x)
    {
        return price24h * x;
    }

    virtual void print() = 0;
};

class Bike: public Vehicle
{
public:
    Bike(int x, string s, float f = 9.99):
        Vehicle(x, s, f)
    {}

    virtual void print()
    {
        cout << ": " << Vehicle::get_model() << Vehicle::get_no() << " (Bike)" << endl;
    }
};

class EMotorVehicle: public Vehicle
{
public:
    EMotorVehicle(int x, string s, float f):
        Vehicle(x, s, f)
    {}

    virtual bool is_streetLegal() = 0;

    virtual void print()
    {
        if(is_streetLegal() == true)
        {
            cout << ": " << Vehicle::get_model() << Vehicle::get_no();
        }
        else
        {
            cout << ": " << Vehicle::get_model() << Vehicle::get_no() << " (not street legal)";
        }
    }
};

class EBike: public EMotorVehicle
{
public:
    EBike(int x, string s, float f = 29.99):
        EMotorVehicle(x, s, f)
    {}

    virtual bool is_streetLegal()
    {
        return true;
    }

    virtual void print()
    {
        EMotorVehicle::print();
        cout << " (EBike)" << endl;
    }
};

class EScooter: public EMotorVehicle
{
private:
    bool streetLegal;

public:
    EScooter(int x, string s, float f = 19.99):
        EMotorVehicle(x, s, f)
    {}

    virtual bool is_streetLegal()
    {
        return false;
    }

    virtual void print()
    {
        EMotorVehicle::print();
        cout << " (EScooter)" << endl;
    }
};

class Rental
{
private:
    const int no;
    DayOfYear from;
    int days;
    string customer;
    static int last_no;

public:
    Rental(string s, DayOfYear d, int x = 1):
        customer(s),
        from(d),
        days(x),
        no(last_no)
    {
        last_no++;
    }

    int get_days()
    {
        return days;
    }

    DayOfYear get_from()
    {
        return from;
    }

    DayOfYear get_until()
    {
        DayOfYear d = from;
        for(int i = 0; i < days; i++)
        {
            ++d;
        }
        return d;
    }

    void print()
    {
        Vehicle * v;
        cout << get_from() << " to " << get_until()
             << ", rental no. " << no << " for " << customer;
    }
};

int Rental::last_no = 1;

class Schedule
{
private:
    Vehicle * vehicle;
    Rental * year[365];
    Schedule * next;

public:
    Schedule(Vehicle * v):
        vehicle(v),
        next(nullptr)
    {
        for(int i = 0; i < 365; i++)
        {
            year[i] = nullptr;
        }
    }

    Vehicle * get_vehicle()
    {
        return vehicle;
    }

    Schedule * get_next()
    {
        return next;
    }

    void set_next(Schedule * s)
    {
        next = s;
    }

    bool isFree(DayOfYear d, int x)
    {
        for(int i = 0; i < x; i++)
        {
            if(year[d.get_dayOfYear()] != nullptr)
            {
                return false;
            }
            else
            {
                ++d;
            }
        }
        return true;
    }

    float book(string s, DayOfYear d, int x)
    {
        Rental * r = new Rental(s, d, x);
        for(int i = 0; i < x; i++)
        {
            year[d.get_dayOfYear() + i] = r;
        }
        return vehicle->get_price(x);
    }

    void print(DayOfYear d)
    {
        vehicle->print();
        if(year[d.get_dayOfYear()] != nullptr)
        {
            year[d.get_dayOfYear()]->print();
            cout << endl;
        }
    }

    void print()
    {
        vehicle->print();
        for(int i = 0; i < 365; i++)
        {
            if(year[i] != nullptr && year[i] != year[i-1])
            {
                year[i]->print();
                cout << ", total " << vehicle->get_price(year[i]->get_days()) << " EUR" << endl;
            }
        }
    }
};

class Agency
{
private:
    float profitPercent, profit;
    Schedule * head, * last;

protected:
    Schedule * searchFor(int x)
    {
        Schedule * s = head;
        int i = 1;
        while(s != nullptr)
        {
            if(i == x)
            {
                return s;
            }
            else
            {
                s = s->get_next();
                i++;
            }
        }
        return s;
    }

public:
    Agency():
        profitPercent(0.2),
        profit(0),
        head(nullptr),
        last(nullptr)
    {}

    float get_profit()
    {
        return profit * profitPercent;
    }

    void set_profitPercentage(float f)
    {
        profitPercent = f;
    }

    void add(Vehicle * v)
    {
        Schedule * s = new Schedule(v);
        if (head == nullptr)
        {
            head = s;
            last = s;
        }
        else
        {
            last->set_next(s);
            last = s;
        }
    }

    bool isFree(int x1, DayOfYear d, int x2)
    {
        Schedule * s = searchFor(x1);
        return s->isFree(d, x2);
    }

    float book(int x1, string s1, DayOfYear d, int x2)
    {
        Schedule * s = searchFor(x1);
        profit += s->book(s1, d, x2);
        return s->get_vehicle()->get_price(x2);
    }

    int chooseVehicle()
    {
        int i = 1;
        Schedule * s = head;
        while(s != nullptr)
        {
            cout << i;
            s->get_vehicle()->print();
            i++;
            s = s->get_next();
        }
    }

    void print(DayOfYear d)
    {
        Schedule * s = head;
        int i = 1;
        cout << "SCHEDULE FOR " << d <<endl;
        while(s != nullptr)
        {
            cout << i;
            s->print(d);
            s = s->get_next();
            i++;
        }
    }

    void print()
    {
        int i = 1;
        Schedule * s = head;
        while(s != nullptr)
        {
            cout << "SCHEDULE FOR " << i;
            s->print();
            i++;
            s = s->get_next();
        }
    }
};

int main(void)
{
    //Initialize agency
    Agency a;

    //Loop functions, Case 'a'
    int x1 = 0;
    char c1;

    //Case 'b'
    DayOfYear d1, d2, d3;
    ++d2;

    //Case 'c'
    int x2, x3;
    DayOfYear d4;
    string s1;

    //Case 'h'
    float f1;

    //Case 'i', 'j', 'k'
    Vehicle * v[100000];
    int counter = 1; //for the vehicle array
    int bq, ebq, esq; //bq = Bike Quantity, etc.
    int countBike = 1;
    int countEBike = 1;
    int countEScooter = 1;

    cout << "SIMPLIFIED SIMULATION OF A RENTAL AGENCY" << endl;
    cout << "========================================" << endl;

    while(x1 < 1)
    {
        cout << "________________________________________\n"
             << "MENU\n"
             << "a - end of simulation\n"
             << "b - set new simulation date\n"
             << "c - new rental manually\n"
             << "d - print rental of today " << d1 << "\n"
             << "e - print rental of tomorrow " << d2 << "\n"
             << "f - print all rentals\n"
             << "g - print agency profit\n"
             << "h - set agency profit percent\n"
             << "i - add bikes\n"
             << "j - add E-bikes\n"
             << "k - add E-scooters\n"
             << "your choice: ";
        cin >> c1;

        switch(c1)
        {
            case 'a':
                x1 = 1;
                break;

            case 'b':
                cout << "Input date of today: ";
                cin >> d3;
                d1 = d3;
                d2 = d3;
                ++d2;
                break;

            case 'c':
                a.chooseVehicle();
                cout << "choose vehicle no: ";
                cin >> x2;
                cout << "day of rental: ";
                cin >> d4;
                cout << "number of days: ";
                cin >> x3;
                cout << "input data vehicle renter name: ";
                cin >> s1;
                if(a.isFree(x2, d4, x3) == true)
                {
                    cout << "booked, price for this rental: " << a.book(x2, s1, d4, x3) << endl;
                }
                else
                {
                    cout << "sorry vehicle is already booked" << endl;
                }
                break;

            case 'd':
                a.print(d1);
                break;

            case 'e':
                a.print(d2);
                break;

            case 'f':
                a.print();
                cout << "profit: " << fixed << setprecision(2) << a.get_profit() << " EUR" << endl;
                break;

            case 'g':
                cout << "agency profit: " << fixed << setprecision(2) << a.get_profit() << " EUR" << endl;
                break;

            case 'h':
                cout << "agency profit percent: ";
                cin >> f1;
                a.set_profitPercentage(f1/100);
                break;

            case 'i':
                cout << "How many bikes in this simulation? ";
                cin >> bq;

                for(int i = 0; i < bq; i++)
                {
                    Bike *b = new Bike(countBike, "City");
                    v[counter] = b;
                    a.add(v[counter]);
                    countBike++;
                    counter++;
                }
                break;

            case 'j':
                cout << "How many Ebikes in this simulation? ";
                cin >> ebq;
                for(int i = 0; i < ebq; i++)
                {
                    EBike *eb = new EBike(countEBike, "Trekky");
                    v[counter] = eb;
                    a.add(v[counter]);
                    countEBike++;
                    counter++;
                }
                break;

            case 'k':
                cout << "How many Escooter in this simulation? ";
                cin >> esq;
                for(int i = 0; i < esq; i++)
                {
                    EScooter *es = new EScooter(countEScooter, "Scootey");
                    v[counter] = es;
                    a.add(v[counter]);
                    countEScooter++;
                    counter++;
                }
                break;

            default:
                cout << "ERROR" << endl;
                break;
        }
    }
}
