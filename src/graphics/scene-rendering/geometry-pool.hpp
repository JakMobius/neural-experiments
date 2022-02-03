#pragma once

#include <map>
#include "../gl/gl-buffer.hpp"
#include "../gl/gl-texture-buffer.hpp"
#include "scene-object-handle.hpp"
#include "scene-object-config.hpp"
#include "../../utils/vec2.hpp"
#include "../../utils/index-pool.hpp"
#include "../../utils/linked-list.hpp"

namespace Graphics {

class GeometryPool {
    std::vector<GLint> start_indices {};
    std::vector<GLsizei> size_array {};

    std::unique_ptr<GLBuffer<float>> m_vertex_buffer {};
    BufferRange m_vertex_buffer_dirty_range {};

    std::unique_ptr<GLTextureBuffer<float>> m_matrix_buffer {};
    BufferRange m_matrix_buffer_dirty_range {};

    IndexPool<int> m_matrix_buffer_index_pool {};
    DoubleEndedLinkedList<GeometryObject> m_objects {};

    std::vector<GeometryObject*> m_dirty_transform_objects {};

    GeometryObject* m_defragmented_elements = nullptr;

    void defragment_single_object(GeometryObject* object, int offset);

    static void extend_buffer(std::vector<float>& buffer, int min_capacity);
    int allocate_buffer(int size);
    int get_free_offset();

    void copy_geometry(int offset, std::vector<SceneVertex> vertices, int matrix_index);

    void insert_offsets_to_arrays(int free_index, int buffer_stride);
    void remove_offset_from_arrays(int offset);
    void adjust_offset_from_arrays(int offset, int new_offset);

    void initialize_matrix(int index);
    void copy_matrix(int index, const Matrix4f& matrix);

public:

    GLint* get_start_indices() { return &start_indices[0]; };
    GLsizei* get_size_array() { return &size_array[0]; };
    int get_object_count() { return m_objects.size(); }

    GeometryPool();

    GeometryObject* create_object(const SceneObjectConfig &object_config, GeometryObject* parent);
    void update_object_transform(GeometryObject* object);
    void remove_object(GeometryObject* object);

    GLBuffer<float>* get_vertex_buffer() { return m_vertex_buffer.get(); };
    GLTextureBuffer<float>* get_matrix_buffer() { return m_matrix_buffer.get(); };

    void synchronize();

    void update_transforms();

    void defragment_buffer(int limit);

    void update_transform_delayed(GeometryObject* object);
};

}