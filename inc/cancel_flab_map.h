#ifndef _CANCEL_FLAB_MAP_H
#define _CANCEL_FLAB_MAP_H

/*
 *    $RCSfile: cancel_flab_map.h,v $
 *    $Revision: 1.2 $
 *    $Date: 2002/09/30 20:55:21 $
 */

/*
 * cancel_flab_map.h:
 * 
 *   build the map which gives the cancelling labels of a label
 *   fully expanded sddd.
 *
 */

// structure for each individual label.
typedef struct _cancelLab
{
	int m_label;
	struct _cancelLab *m_next;
} CancelLab;


class CancelFLabMap 
{
public:
	class CancelLabList
	{
	public:
		CancelLabList();
		~CancelLabList();

		void add_label(int label);
		CancelLab * get_label_list();
		bool        empty() { return (get_label_list() == NULL); }
		void print_label_list();

	private:
		CancelLab *m_cancel_lab_list;
	};

	CancelFLabMap(int size);
	~CancelFLabMap();

	void add_cancel_lab_pair(int lab, int cancal_lab);
	CancelLab * get_cancel_lab_list(int lab);
	void print_cancel_lab_map();

private:
	int m_size;
	CancelLabList *m_list_vec;
};

extern CancelFLabMap *theCancelFLabMap;

#endif //_CANCEL_FLAB_MAP_H
