namespace gossip {
    int valueData::setValue(int amt) { return value = amt; }
    int valueData::addValue(int amt) { return value += amt; }
    int valueData::removeValue(int amt) { return value -= amt; }
    int valueData::getValue() { return value; }
    void valueData::setValueMin(int amt) { min = amt; }
    void valueData::setValueMax(int amt) { max = amt; }
    int valueData::getValueMin() { return min; }
    int valueData::getValueMax() { return max; }
}