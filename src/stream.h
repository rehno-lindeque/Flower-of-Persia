#ifndef __STREAM_H__
#define __STREAM_H__

/* A stream is meant to be used as follows: Stream << (Stream << Stream << Stream) << Stream etc...
   Essentially, it's a buffer for easy insertion (and sub-insertion in recursive types of methods (See Geometry))
*/

//TODO: this isn't extremely efficient: could use the memory resize method used in Chimera Engine instead in future
template<class Type>
class Stream
{
protected:
  Type* array;
  uint length;
  bool dynamic;

public:
  inline Stream() : array(null), length(0), dynamic(false) {}
  inline Stream(Type* array, uint length) : array(array), length(length), dynamic(false) {}
  inline Stream(const Stream<Type>& clone) : array(clone.array), length(clone.length), dynamic(false) {}

  inline ~Stream() { if(dynamic) delete[] array; }

  inline uint getLength() const { return length; }
  inline operator Type* () { return array; }
  inline operator const Type* () const { return array; }

  Stream& operator << (const Stream<Type>& arg)
  {
    // copy original data
    Type* oldArray = array;
    array = new Type[length + arg.length];
    memcpy(array, oldArray, length*sizeof(Type));
    if(dynamic)
      delete[] oldArray;
    dynamic = true;

    // append argument data
    memcpy(array+length, (const Type*)arg, arg.length*sizeof(Type));
    length += arg.length;

    return *this;
  }

};

#endif
