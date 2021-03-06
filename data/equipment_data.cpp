#include "equipment_data.h"

Ref<EquipmentDataEntry> EquipmentData::get_slot(int index) {
	ERR_FAIL_INDEX_V(index, ItemEnums::EQUIP_SLOT_EQUIP_SLOT_MAX, Ref<EquipmentDataEntry>());

	return _entries[index];
}
void EquipmentData::set_slot(int index, Ref<EquipmentDataEntry> entry) {
	ERR_FAIL_INDEX(index, ItemEnums::EQUIP_SLOT_EQUIP_SLOT_MAX);

	_entries[index] = entry;
}

Ref<ItemInstance> EquipmentData::get_item(int index) {
	ERR_FAIL_INDEX_V(index, ItemEnums::EQUIP_SLOT_EQUIP_SLOT_MAX, Ref<ItemInstance>());

	Ref<EquipmentDataEntry> ede = _entries[index];

	if (!ede.is_valid())
		return Ref<ItemInstance>();

	return ede->get_item();
}

EquipmentData::EquipmentData() {
}

EquipmentData::~EquipmentData() {
	for (int i = 0; i < ItemEnums::EQUIP_SLOT_EQUIP_SLOT_MAX; ++i)
		_entries[i].unref();
}

void EquipmentData::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_slot", "index"), &EquipmentData::get_slot);
	ClassDB::bind_method(D_METHOD("set_slot", "index", "entry"), &EquipmentData::set_slot);

	ADD_GROUP("Slots", "slot");
	for (int i = 0; i < ItemEnums::EQUIP_SLOT_EQUIP_SLOT_MAX; ++i) {
		ADD_PROPERTYI(PropertyInfo(Variant::OBJECT, "slot_" + ItemEnums::get_equip_slot_string(i), PROPERTY_HINT_RESOURCE_TYPE, "EquipmentDataEntry"), "set_slot", "get_slot", i);
	}
}
