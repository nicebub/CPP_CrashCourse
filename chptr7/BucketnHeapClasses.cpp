#include <cstddef>
#include <cstdio>
#include <new>
struct Bucket {
	
	const static size_t data_size{ 4096 };
	std::byte data[data_size];
};
struct LargeBucket {
	
	const static size_t data_size{ 1048576 };
	std::byte data[data_size];
};

struct Heap {
	void* allocate(size_t bytes){
		if(bytes > LargeBucket::data_size) throw std::bad_alloc{};
		if(bytes<=Bucket::data_size){
			for(size_t i{}; i < n_heap_buckets; i++){
				if(!bucket_used[i]){
					bucket_used[i] = true;
					return buckets[i].data;
				}
			}
		}
		else{
			for(size_t i{}; i < n_heap_buckets; i++){
				if(!lbucket_used[i]){
					lbucket_used[i] = true;
					return lbuckets[i].data;
				}
			}
		}
		throw std::bad_alloc{};
	}


void free(void* p) {
	for (size_t i{}; i < n_heap_buckets; i++) {
		if(buckets[i].data == p) {
			bucket_used[i] = false;
			return;
		}
		else if(lbuckets[i].data == p){
			lbucket_used[i] = false;
			return;
		}
	}
}

static const size_t n_heap_buckets { 10 };
Bucket buckets[n_heap_buckets]{};
LargeBucket lbuckets[n_heap_buckets]{};
bool bucket_used[n_heap_buckets]{};
bool lbucket_used[n_heap_buckets]{};
};

Heap heap;

void* operator new(size_t n_bytes){
	return heap.allocate(n_bytes);
}

void operator delete(void* p) noexcept {
	return heap.free(p);
}

int main(){
	printf("Buckets:   %p\n",heap.buckets);
	auto breakfast = new unsigned int{ 0xC0FFEE };
	auto dinner = new unsigned int {  0XDEADBEEF };
	printf("Breakfast : %p 0x%x\n", breakfast, *breakfast);
	printf("Dinner : %p 0x%x\n", dinner, *dinner);
	delete breakfast;
	delete dinner;
	
	auto lbholder1 = new unsigned int[4097]{ 0xdeaddead };
	printf("lbholder1 : %p 0x%x\n", lbholder1, *lbholder1);
	delete[] lbholder1;
	try{
		while (true){
			new char;
		}
	} catch (const std::bad_alloc&){
		printf("std::bad_alloc caught.\n");
	}
	try{
		while (true){
			new char[4097];
		}
	} catch (const std::bad_alloc&){
		printf("std::bad_alloc caught for Large Bucket.\n");
	}

}