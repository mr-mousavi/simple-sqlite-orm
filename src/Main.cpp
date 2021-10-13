#include "Student.h"
#include <iostream>


int main()
{
	//save a student
	Student st;
	st.setId(1);
	st.setFirstName("student name");
        st.setLastName("student last name");
    	st.setNationcode("123456789");
	st.setBirthDate("1987/05/13");
	st.save();

	//load a student
	Student st2;
	st2.load();
	st2.print();

}




