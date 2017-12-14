#ifndef CONTROL_H
#define CONTROL_H

#include <map>
#include <string>

typedef std::string ControlName;


class Control
{
	typedef std::map<ControlName, double> ControlMap;
	
    ControlMap control;
	const ControlMap &GetControlMap() const;
	double &operator[] ( const ControlName & );

public:
	
	typedef ControlMap::iterator iterator;
	typedef ControlMap::const_iterator const_iterator;
	
    explicit Control ( const ControlMap & );
	Control (const Control&);
    Control();
    int Size() const;
    friend MyLogger &operator<< ( MyLogger &, const Control & );
    bool operator== ( const Control & ) const;
    bool operator!= ( const Control & ) const;
    Control &operator= ( const Control & );
	
	ControlMap::iterator begin();
	ControlMap::iterator end();
	ControlMap::const_iterator begin() const;
	ControlMap::const_iterator end() const;
	ControlMap::const_iterator find(const ControlName &) const;
	const double& at ( const std::string & ) const;
    double &at ( const std::string & );
	const double& operator() ( const std::string & ) const;
    double & operator() ( const std::string & );

	static Control GenerateControlOfType( const Control & );

    friend void CheckConsistency ( const std::string &, const Control &, const Control & );
};

typedef std::vector<Control> ControlVector;


MyLogger &operator<< ( MyLogger &, const Control & );

void CheckConsistency ( const std::string &, const Control &, const Control & );

#endif
