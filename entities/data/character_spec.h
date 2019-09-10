#ifndef CHARACTER_SPEC_H
#define CHARACTER_SPEC_H

#include "core/resource.h"
#include "core/ustring.h"
#include "core/vector.h"

#include "talent_row_data.h"

class TalentRowData;
class Talent;

class CharacterSpec : public Resource {
	GDCLASS(CharacterSpec, Resource);

public:
	int get_spec_id();
	void set_spec_id(int value);
	String get_spec_name();
	void set_spec_name(String value);

	int get_num_talent_rows();
	void set_num_talent_rows(int value);

	Ref<TalentRowData> get_talent_row(int index) const;
	void set_talent_row(const int index, const Ref<TalentRowData> row);

	Vector<Variant> get_talent_rows();
	void set_talent_rows(const Vector<Variant> &auras);

	Ref<Talent> get_talent(const int row_index, const int index) const;

	CharacterSpec();
	~CharacterSpec();

protected:
	static void _bind_methods();

private:
	int _spec_id;
	String _spec_name;
	Vector<Ref<TalentRowData> > _rows;
};

#endif