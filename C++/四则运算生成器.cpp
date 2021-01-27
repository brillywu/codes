#include <iostream>
#include <string>
#include <ctime>
#include <cstdlib> // Header file needed to use srand and rand
#include <sstream>

using namespace std;

#define ARRAY_SIZE(x) (sizeof(x)/sizeof((x)[0]))

unsigned int generateValue(unsigned int MIN_VALUE=1,unsigned int MAX_VALUE=1000){
    return rand() % (MAX_VALUE - MIN_VALUE + 1) + MIN_VALUE;    
}

unsigned int generateOperation(){
    const unsigned int ops[]={1,2,3,4,4,1,2,3,4,4,4,1,2,3,4,4,4,1,2,3,4,4,4,4,1,2,3,4,4,4};
    return ops[ generateValue(0,ARRAY_SIZE(ops)-1) ];
}

unsigned int generateLevel(int MAX_VALUE){
    return generateValue(0,MAX_VALUE);
}


class Value;
class Plus;
class Minus;
class Mult;
class Division;

class Express{
public:    
    virtual int priority()=0; //表达式运算符的优先级。+，-为level1，x / 为level2
    virtual unsigned int value()=0;
    virtual string show()=0;
    virtual bool ok()=0;
    virtual int operator_count()=0; // 表达式由几个运算符组成。( a+b ) - ( c+d ) ==> 3
};

class Value : public Express{
    unsigned int v;
public:
    Value(unsigned int _v){v=_v;}
    bool ok() { return v>=0;}
    unsigned int value() { return v;}
    int priority(){return 100;}
    string show(){
        std::stringstream buf;
        buf.precision(2);//覆盖默认精度
        buf.setf(std::ios::fixed);//保留小数位
        buf << v;
        return buf.str();
    }

    int operator_count(){return 0;}
};


class Operator: public Express{
public:    
    Express* left;
    Express* right;
public:    
    Operator(Express* l,Express* r){left=l;right=r;}    
    bool ok() { return left->value()>=0 && right->value() >=0 ; }
    int operator_count(){ return left->operator_count() + right->operator_count() +1;}
};

class Plus : public Operator{
public:
    Plus(Express*l,Express* r): Operator(l,r){}
    int priority(){return 1;}
    
    unsigned int value() {
        return left->value() + right->value();
    }

    string show(){
        std::stringstream buf;
        buf.precision(2);//覆盖默认精度
        buf.setf(std::ios::fixed);//保留小数位

        buf <<   left->show() << "+" << right->show();
        return buf.str();
    }
};

class Minus : public Operator{
    
public:
    Minus(Express*l,Express* r)    
    :Operator( new Value( Plus(l,r).value() ), r)
    {}
    int priority(){return 1;}

    bool ok() { return (left->value() > right->value()) && Operator::ok() ; }
    
    unsigned int value() {
        return left->value() - right->value();
    }

    string show(){
        std::stringstream buf;
        buf.precision(2);//覆盖默认精度
        buf.setf(std::ios::fixed);//保留小数位

        buf << left->show() << "-";
        if(right->priority()==1)
            buf<< "(" <<  right->show() << ")";
        else 
            buf<< right->show() ;
        return buf.str();
    }
};

class Mult : public Operator{
    
public:
    Mult(Express*l,Express* r): Operator(l,r){}


    int priority(){return 2;}

    bool ok() { 
        return ( (value() !=0) && Operator::ok() ) ;
    }

    unsigned int value() {
        return (left->value() * right->value()) ;
    }    

    string show(){
        string s= left->show() + "x" + right->show();
        return s;
    }
};


class Division : public Operator{
    const unsigned int SSS=1000;
public:
    // l * r = div
    // 
    // 
    Division(Express*l,Express* r): Operator( new Value( Mult(l,r).value() ), r){}

    bool ok() {
        return true;
    }

    int priority(){return 2;}
    
    unsigned int value() {
        return (left->value() / right->value()) ;
    }

    string show(){
        std::stringstream buf;
        buf.precision(2);//覆盖默认精度
        buf.setf(std::ios::fixed);//保留小数位

        buf << left->show() << "➗";
        if(right->priority()<=2)
            buf << "(" <<  right->show() << ")";
        else 
            buf <<  right->show() ;
        return buf.str();
    }
};



static Express* createExpression(int op,Express* left,Express* right){
    switch(op){
        case 1:            
        return new Plus(left, right);
        case 2:
        return new Minus(left, right);
        case 3: 
        return new Mult(left, right);
        case 4:
        if( right->value() == 0 ) return nullptr;
        return new Division(left, right);
    }
    return nullptr;
}

static Express* generateValueExp(){
    return new Value(generateValue(50,100));
}


static Express* generateExpress(int op,Express* l,Express* r)
{
    Express* exp=nullptr;
    do
    {
        exp=createExpression(op, l, r );
    } while (!exp);
    return exp;
}


static Express* generate1Oper(int op)
{
    return generateExpress(op,generateValueExp(),generateValueExp() );
}

    
static Express* generate(int level,int op){        
    if (level>=1 )
    {
        Express* result=nullptr;
        do{
            unsigned int left_level = generateLevel(level-1);
            unsigned int right_level = generateLevel(level-1);
            
            int left_op = generateOperation();
            int right_op = generateOperation();
            Express* left = generate( left_level,left_op  );
            Express* right = generate( right_level  ,right_op );
            
            result= generateExpress( op , left, right );                
        }while(!result->ok());

        return result;
    } else if(level==0) {            
        return generate1Oper(op);
    }
    
    return nullptr;
}



int main() {
    unsigned seed;  // Random generator seed
    // Use the time function to get a "seed” value for srand
    seed = time(0);
    srand(seed);

    

    cout.precision(2);
    const int ops[]={1,2,3,4};
    const int s = ARRAY_SIZE(ops);

    for(int i=0;i<10;i++){
        Express* e = generate(2, ops[ i % s]);
        cout<< e->show() << endl;
    }
}
