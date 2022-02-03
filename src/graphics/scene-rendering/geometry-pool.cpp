//
// Created by Артем on 02.02.2022.
//

#include "geometry-pool.hpp"

namespace Graphics {

void GeometryPool::defragment_single_object(GeometryObject* object, int offset) {
    auto &storage = m_vertex_buffer->get_storage();

    object->m_vertex_buffer_offset -= offset;

    adjust_offset_from_arrays(object->m_vertex_buffer_offset + offset, object->m_vertex_buffer_offset);
    memmove(&storage[object->m_vertex_buffer_offset], &storage[object->m_vertex_buffer_offset + offset],
            offset * sizeof(float));

    m_vertex_buffer_dirty_range.extend(object->m_vertex_buffer_offset, object->m_vertex_buffer_offset + object->m_vertex_buffer_length);
}

void GeometryPool::defragment_buffer(int limit) {
    if(m_defragmented_elements == nullptr) return;

    GeometryObject* previous = m_defragmented_elements;
    m_defragmented_elements = m_defragmented_elements->m_next;

    if(previous->m_vertex_buffer_offset > 0) {
        defragment_single_object(previous, previous->m_vertex_buffer_offset);
        if(!(--limit)) return;
    }

    int free_offset = previous->m_vertex_buffer_length;

    for(; m_defragmented_elements; m_defragmented_elements = m_defragmented_elements->m_next) {
        GeometryObject* next = m_defragmented_elements;

        if(next->m_vertex_buffer_offset > free_offset) {
            defragment_single_object(previous, free_offset - next->m_vertex_buffer_offset);
            if(!(--limit)) return;
        }

        previous = next;
    }
}

void GeometryPool::extend_buffer(std::vector<float>& buffer, int min_capacity) {
    auto capacity = buffer.size();
    if(capacity >= min_capacity) return;
    if(capacity == 0) capacity = 128;

    while(capacity < min_capacity) {
        capacity *= 2;
    }

    buffer.resize(capacity);
}

int GeometryPool::get_free_offset() {
    if(m_objects.empty()) return 0;

    return m_objects.end()->m_vertex_buffer_offset + m_objects.end()->m_vertex_buffer_length;
}

int GeometryPool::allocate_buffer(int size) {
    auto free_index = get_free_offset();
    auto& storage = m_vertex_buffer->get_storage();

    if(free_index + size >= storage.size()) {
        defragment_buffer(INT_MAX);
        free_index = get_free_offset();
        extend_buffer(storage, free_index + size);
    }

    return free_index;
}

GeometryPool::GeometryPool() {
    m_vertex_buffer = std::make_unique<GLBuffer<float>>(GLBufferType::array_buffer, GLBufferUsage::static_draw);
    m_matrix_buffer = std::make_unique<GLTextureBuffer<float>>(GLTextureInternalFormat::rgba32f, GLBufferUsage::static_draw);

    m_vertex_buffer->create_buffer();
    m_matrix_buffer->create_buffer();
}

void GeometryPool::insert_offsets_to_arrays(int free_index, int buffer_stride) {
    start_indices.push_back(free_index / SceneVertex::length);
    size_array.push_back(buffer_stride / SceneVertex::length);
}

void GeometryPool::remove_offset_from_arrays(int offset) {
    offset /= SceneVertex::length;

    auto array_it = std::lower_bound(start_indices.begin(), start_indices.end(), offset) - start_indices.begin();

    start_indices.erase(start_indices.begin() + array_it);
    size_array.erase(size_array.begin() + array_it);

    std::cout << "Removed array entry on index " << (array_it) << "\n";
}

void GeometryPool::adjust_offset_from_arrays(int offset, int new_offset) {
    offset /= SceneVertex::length;
    new_offset /= SceneVertex::length;

    auto array_it = std::lower_bound(start_indices.begin(), start_indices.end(), offset);

    *array_it = new_offset;
}

void GeometryPool::remove_object(GeometryObject* object) {
    for(auto& child : object->m_children) {
        remove_object(child);
    }

    object->m_children.clear();

    auto old_defragmented_handle = m_defragmented_elements;

    remove_offset_from_arrays(object->m_vertex_buffer_offset);

    m_matrix_buffer_index_pool.release_index(object->m_matrix_buffer_index);

    std::cout << "Deleted object handle on index " << object->m_vertex_buffer_offset << " with stride of " << object->m_vertex_buffer_length << " floats\n";

    if(!old_defragmented_handle || old_defragmented_handle->m_matrix_buffer_index >= object->m_matrix_buffer_index) {
        m_defragmented_elements = object->m_prev;
    }

    m_objects.erase(object);
    delete object;
}

void GeometryPool::update_object_transform(GeometryObject* object) {
    auto index = object->m_matrix_buffer_index;
    copy_matrix(index, object->m_world_transform);
}

GeometryObject* GeometryPool::create_object(const SceneObjectConfig &object_config, GeometryObject* parent) {
    auto buffer_stride = (int) object_config.m_mesh.size() * SceneVertex::length;
    auto free_index = allocate_buffer(buffer_stride);

    std::cout << "Adding object handle on index " << free_index << " with stride of " << buffer_stride << " floats\n";

    auto matrix_index = m_matrix_buffer_index_pool.get_next();
    initialize_matrix(matrix_index);

    auto object = new GeometryObject(this, free_index, buffer_stride, matrix_index, parent);
    m_objects.push_back(object);
    object->set_needs_transform_update();

    insert_offsets_to_arrays(free_index, buffer_stride);

    copy_geometry(free_index, object_config.m_mesh, matrix_index);

    return object;
}

void GeometryPool::synchronize() {
    if(!m_vertex_buffer_dirty_range.is_empty()) {
        m_vertex_buffer->synchronize(m_vertex_buffer_dirty_range);
        m_vertex_buffer_dirty_range.clear();
    }

    if(!m_matrix_buffer_dirty_range.is_empty()) {
        m_matrix_buffer->synchronize(m_matrix_buffer_dirty_range);
        m_matrix_buffer_dirty_range.clear();
    }
}

void GeometryPool::initialize_matrix(int index) {
    const int matrix_size = 16;
    index *= matrix_size;

    auto& storage = m_matrix_buffer->get_storage();
    extend_buffer(storage, index + matrix_size);

    for(int i = 0; i < matrix_size; i++) {
        storage[index + i] = ((i % 4) == (i / 4)) ? 1 : 0;
    }

    m_matrix_buffer_dirty_range.extend(index, index + 16);
}

void GeometryPool::copy_matrix(int index, const Matrix4f& matrix) {
    const int matrix_size = 16;
    index *= matrix_size;

    auto& storage = m_matrix_buffer->get_storage();

    for(int i = 0; i < matrix_size; i++) {
        storage[index + i] = matrix.m_data[i];
    }

    m_matrix_buffer_dirty_range.extend(index, index + 16);
}

void GeometryPool::copy_geometry(int offset, std::vector<SceneVertex> vertices, int matrix_index) {
    auto buffer_stride = (int) vertices.size() * SceneVertex::length;
    auto &storage = m_vertex_buffer->get_storage();

    for(auto &vertex : vertices) {
        memcpy(&storage[offset], &vertex, sizeof(SceneVertex));
        *((int*)&storage[offset + SceneVertex::index_offset]) = matrix_index;
        offset += SceneVertex::length;
    }

    m_vertex_buffer_dirty_range.extend(offset, offset + buffer_stride);
}

void GeometryPool::update_transforms() {
    for(auto& object : m_dirty_transform_objects) {
        object->update_transform();
        object->update_subsidiary_transformations();
    }
    m_dirty_transform_objects.clear();
}

void GeometryPool::update_transform_delayed(GeometryObject* object) {
    m_dirty_transform_objects.push_back(object);
}

}