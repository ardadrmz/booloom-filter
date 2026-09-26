#include <string>
#include <cstdint>
#include <cstring>

using namespace std;

const int MAX_SIZE = 7500;

bool bit_array[MAX_SIZE];

void init_bitArray(){
    for(int i = 0;i < MAX_SIZE;i++){
        bit_array[i] = false;
    }
}

uint32_t djb2(string data) {  
    uint32_t hash = 5381;
    for (char c : data) {
        hash = ((hash << 5) + hash) + (uint8_t)c;
    }
    return hash;
}

uint32_t jenkins_one_at_a_time(string data) {   
    uint32_t hash = 0;
    for (char c : data) {
        hash += (uint8_t)c;
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }
    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);
    return hash;
}

static inline uint32_t murmur_32_scramble(uint32_t k) {
    k *= 0xcc9e2d51;
    k = (k << 15) | (k >> 17);
    k *= 0x1b873593;
    return k;
}

uint32_t murmur_hash3(const uint8_t* key, size_t len, uint32_t seed) {
    uint32_t h = seed;
    uint32_t k;
    for (size_t i = len >> 2; i; i--) {
        memcpy(&k, key, sizeof(uint32_t));
        key += sizeof(uint32_t);
        h ^= murmur_32_scramble(k);
        h = (h << 13) | (h >> 19);
        h = h * 5 + 0xe6546b64;
    }
    k = 0;
    for (size_t i = len & 3; i; i--) {
        k <<= 8;
        k |= key[i - 1];
    }
    h ^= murmur_32_scramble(k);
    h ^= len;
    h ^= h >> 16;
    h *= 0x85ebca6b;
    h ^= h >> 13;
    h *= 0xc2b2ae35;
    h ^= h >> 16;
    return h;
}
uint32_t hash_string(const string& str, uint32_t seed) { 
    return murmur_hash3(reinterpret_cast<const uint8_t*>(str.data()), str.size(), seed);
}

void record_to_filter(string data){
    bit_array[djb2(data) % MAX_SIZE] = true;
    bit_array[jenkins_one_at_a_time(data) % MAX_SIZE] = true;
    bit_array[hash_string(data,1) % MAX_SIZE] = true;
    bit_array[hash_string(data,2) % MAX_SIZE] = true;
    bit_array[hash_string(data,3) % MAX_SIZE] = true;
}
bool check_data(string data){

    if(bit_array[djb2(data) % MAX_SIZE] == false){
        return false;
    }
    if(bit_array[jenkins_one_at_a_time(data) % MAX_SIZE] == false){
        return false;
    }
    if(bit_array[hash_string(data,1) % MAX_SIZE] == false){
        return false;
    }
    if(bit_array[hash_string(data,2) % MAX_SIZE] == false){
        return false;
    }
    if(bit_array[hash_string(data,3) % MAX_SIZE] == false){
        return false;
    }
    return true;
}

int main(){    
    return 0;
}