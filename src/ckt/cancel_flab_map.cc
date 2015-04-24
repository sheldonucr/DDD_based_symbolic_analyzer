/*
*******************************************************

        Symbolic Circuit Analysis With DDDs
                (*** SCAD3 ***)

   Xiang-Dong (Sheldon) Tan, 1998 (c) Copyright

   Electrical Engineering, Univ. of Washington
   Electrical Computer Engineering, Univ. of Iowa

   email: xtan@ee.washington.edu and xtan@eng.uiowa.edu

*******************************************************
*/

/*
 *    $RCSfile: cancel_flab_map.cc,v $
 *    $Revision: 1.1 $
 *    $Date: 2001/11/25 07:12:30 $
 */


/*
 * cancel_flab_map.cc:
 * 
 *   build the map which gives the cancelling labels of a label
 *   fully expanded sddd.
 *
 */

/* INCLUDES: */

#include "globals.h"
#include "cancel_flab_map.h"

/* global variables */
CancelFLabMap *theCancelFLabMap = NULL;

/*****************************************************
member function of CancalFLabMap
******************************************************/

CancelFLabMap::CancelLabList::CancelLabList()
{
	m_cancel_lab_list = NULL;
}

CancelFLabMap::CancelLabList::~CancelLabList()
{
	if(m_cancel_lab_list)
	{
		CancelLab *tmp_lab = m_cancel_lab_list;
		while(tmp_lab)
		{
			CancelLab *next_lab = tmp_lab->m_next;
			delete tmp_lab;
			tmp_lab = next_lab;
		}
	}
}

void
CancelFLabMap::CancelLabList::add_label
(
	int label
)
{
	CancelLab *tmp = m_cancel_lab_list;
	if(!tmp)
	{
		tmp = new CancelLab;
		tmp->m_label = label;
		tmp->m_next = NULL;
		m_cancel_lab_list = tmp;
	}
	else
	{
		CancelLab *prev = tmp;
		for(; tmp != NULL; tmp = tmp->m_next)
		{
			if(tmp->m_label == label)
			{
				break; // already exists, do nothing
			}
			prev = tmp;
		}
		if(!tmp)
		{
			assert(prev);
			CancelLab * new_lab = new CancelLab;
			new_lab->m_label = label;
			new_lab->m_next = NULL;
			prev->m_next = new_lab;
		}
	}
}

CancelLab *
CancelFLabMap::CancelLabList::get_label_list()
{
	return m_cancel_lab_list;
}

void
CancelFLabMap::CancelLabList::print_label_list()
{
	if(m_cancel_lab_list)
	{
		CancelLab *canlab = m_cancel_lab_list;		
		for(; canlab != NULL; canlab = canlab->m_next)
		{
			cout << canlab->m_label <<" ";
		}
	}
}

/////////////////////////////////////////////////////////
// CancelFLabMap
/////////////////////////////////////////////////////////

CancelFLabMap::CancelFLabMap
(
	int size
)
{
	m_size = size;

	m_list_vec = new CancelLabList[m_size];
}

CancelFLabMap::~CancelFLabMap()
{
	if(m_list_vec)
	{
		delete [] m_list_vec;
	}
}

void
CancelFLabMap::add_cancel_lab_pair
(
	int lab,
	int cancel_lab
)
{	
	int tmp;

	if(lab >= m_size)
	{
		cout << "lab:" << lab
			 << " m_size:" << m_size << endl;
		assert(lab < m_size);
	}
	if(cancel_lab >= m_size)
	{
		cout << "cancel_lab:" << cancel_lab
			 << " m_size:" << m_size << endl;
	
		assert(cancel_lab < m_size);
	}

	// we need make sure lab > cancel_lab;
	if (lab < cancel_lab)
	{
		tmp = lab;
		lab = cancel_lab;
		cancel_lab = tmp;
	}

	assert(lab > cancel_lab);
	
	cout << "add label: " << lab
		 << " cancel label: " << cancel_lab << endl;
	m_list_vec[lab].add_label(cancel_lab);
}

CancelLab *
CancelFLabMap::get_cancel_lab_list
(
	int lab
)
{
	assert(lab < m_size);

	return m_list_vec[lab].get_label_list();
}

void
CancelFLabMap::print_cancel_lab_map()
{
	cout << "dump label and its cancelling labels ... " << endl;
	int i;
	for(i = 0; i < m_size; i++)
	{
		if(!m_list_vec[i].empty())
		{
			cout << " label: " << i 
				 << " --> ";
			m_list_vec[i].print_label_list();
			cout << endl;
		}
	}
	
}
