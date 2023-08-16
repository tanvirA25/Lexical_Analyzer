
#include "val.h"
#include <iostream>
using namespace std;


Value Value:: operator+(const Value& op) const {
       
		if(IsInt() && op.IsInt())
		{
			return Value(GetInt() + op.GetInt());
		}
		else if(IsReal() && op.IsReal())
		{
			return Value(GetReal()+ op.GetReal());
		}
		else if( IsReal() && op.IsInt() )
        {
        	return Value(GetReal() + op.GetInt());
		}
        else if (IsInt() && IsReal()){
            return Value(GetInt() + op.GetReal());
        }
		return Value();
		
    }

Value Value:: operator-(const Value& op) const {
       
		if(IsInt() && op.IsInt())
		{
			return Value( (bool) (GetReal() - ((float) op.GetInt())));
		}
		else if(IsInt() && op.IsReal())
		{
			return Value( (bool) (((float) GetInt())-op.GetReal()));
		}
		else if( GetType() == op.GetType() )
        {
        	if( IsReal() )
				 return Value( (bool) (Rtemp - op.GetReal() ));
			if( IsInt() ) 
				return Value( (bool)(Itemp - op.GetInt() ));
		}
		return Value();
		
    }

  Value Value::operator*(const Value& op) const {
      
		if(IsReal() && op.IsInt())
		{
			return Value(GetReal() * (float) op.GetInt());
		}
		else if(IsInt() && op.IsReal())
		{
			return Value( (float) GetInt() * op.GetReal());
		}
		else if( GetType() == op.GetType() )
        {
        	
        	if( IsReal() ) 
				return Value( Rtemp * op.GetReal() );
			if( IsInt() ) 
				return Value( Itemp * op.GetInt() );
		}
		
		return Value();
		
    }

Value Value::operator/ (const Value& op) const {
      
		if(IsReal() && op.IsInt())
		{
			return Value(GetReal() / (float) op.GetInt());
		}
		else if(IsInt() && op.IsReal())
		{
			return Value( (float) GetInt() / op.GetReal());
		}
		else if( GetType() == op.GetType() )
        {
        	
        	if( IsReal() ) 
				return Value( Rtemp / op.GetReal() );
			if( IsInt() ) 
				return Value( Itemp / op.GetInt() );
		}
		
		return Value();
		
    }

    
Value Value:: operator &&(const Value& op) const {
       Value e;
    if(this->IsBool()){
        if(op.IsBool()){
            return this->GetBool() == op.GetBool();
        }
        else 
            return false;
      
    }
    
    return e;
  
    
}

Value Value:: operator ||(const Value& op) const {
       Value e;
    if(this->IsBool()){
        if(op.IsBool()){
            return this->GetBool() || op.GetBool();
        }
        
    }
    
    
    return e;
 
}

Value Value:: operator !() const {
       Value e;
    if(this->IsBool()){
        return Value(!this->GetBool());
        }
 
    return e;
   
}


Value Value:: operator ==(const Value& op) const {
       Value e;
    if(this->IsBool()){
        if(op.IsBool()){
            return this->GetBool() == op.GetBool();
        }
    }
    else if (this->IsInt() && op.IsInt()){
        return this->GetInt() == op.GetInt();
    }
    else if (this->IsReal() && op.IsReal()){
        return this->GetReal() == op.GetReal();
    }
    else if (this->IsInt() && op.IsReal()){
        return this->GetInt() == op.GetReal();
      
    }
    else if (this->IsReal() && op.IsInt()){
        return this->GetReal() == op.GetInt();
    }
    
    
    return e;
  
    
}
    

Value Value:: operator<(const Value& op) const {
       
		if(IsReal() && op.IsInt())
		{
			return Value( (bool) (GetReal() < ((float) op.GetInt())));
		}
		else if(IsInt() && op.IsReal())
		{
			return Value( (bool) (((float) GetInt())<op.GetReal()));
		}
		else if( GetType() == op.GetType() )
        {
        	if( IsString()) 
				return Value( (bool) (Stemp < op.GetString()));
        	if( IsReal() )
				 return Value( (bool) (Rtemp < op.GetReal() ));
			if( IsInt() ) 
				return Value( (bool)(Itemp < op.GetInt() ));
		}
		return Value();
		
    }
    
    
Value Value:: operator>(const Value& op) const {
       
		if(IsReal() && op.IsInt())
		{
			return Value( (bool) (GetReal() > ((float) op.GetInt())));
		}
		else if(IsInt() && op.IsReal())
		{
			return Value( (bool) (((float) GetInt())>op.GetReal()));
		}
		else if( GetType() == op.GetType() )
        {
        	if( IsString()) 
				return Value( (bool) (Stemp > op.GetString()));
        	if( IsReal() )
				 return Value( (bool) (Rtemp > op.GetReal() ));
			if( IsInt() ) 
				return Value( (bool)(Itemp > op.GetInt() ));
		}
		return Value();
		
    }


