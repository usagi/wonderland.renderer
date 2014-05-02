#pragma once

#include <map>
#include <unordered_map>
#include <chrono>

#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "helper.hxx"

namespace wonder_rabbit_project
{
  namespace wonderland
  {
    namespace renderer
    {
      namespace model
      {
        struct animation_state_t
        {
          std::string              name;
          float                    time_in_seconds;
          float                    weight;
          
          explicit animation_state_t()
            : weight( 1.0f )
          { }
          
          animation_state_t
          ( const std::string& n
          , const float t = 0.0f
          , const float w = 1.0f
          )
            : name( n )
            , time_in_seconds( t )
            , weight( w )
          { }
          
          animation_state_t
          ( const std::string& n
          , const std::chrono::nanoseconds& t = std::chrono::nanoseconds( 0 )
          , const float w = 1.0f
          )
            : name( n )
            , time_in_seconds( float( std::chrono::duration_cast< std::chrono::nanoseconds >( t ).count() ) * float( std::chrono::nanoseconds::period::den ) )
            , weight( w )
          { }
          
          template < class T_duration = std::chrono::nanoseconds >
          inline auto time() const
            -> T_duration
          { return std::chrono::duration_cast< T_duration >( std::chrono::nanoseconds( unsigned( time_in_seconds * std::chrono::nanoseconds::period::den ) ) ); }
          
          template < class T_duration >
          inline auto time( const T_duration& t )
            -> void
          { time_in_seconds = float( std::chrono::duration_cast< std::chrono::nanoseconds >( t ).count() ) / float( std::chrono::nanoseconds::period::den ); }
          
          inline auto operator+=( const float delta_time_in_seconds )
            -> void
          { time_in_seconds += delta_time_in_seconds; }
          
          template < class T_duration >
          inline auto operator+=( const T_duration& delta_time )
            -> void
          { time( time() + delta_time ); }
          
          template < class T_duration >
          inline auto operator=( const T_duration& t )
            -> void
          { time( t ); }
          
        };
        
        using animation_states_t = std::list< animation_state_t >;
        
        struct animation_t
        {
          
          struct channel_t
          {
            std::map< float, glm::vec3 > scalings;
            std::map< float, glm::quat > rotations;
            std::map< float, glm::vec3 > translations;
          };
          
          float duration;
          float ticks_per_second;
          
          // bone_name, channel
          std::unordered_map< std::string, channel_t > channels;
          
          auto transformation( const std::string& bone_name, const float time_in_seconds ) const
            -> glm::mat4
          {
            if ( channels.find( bone_name ) == channels.end() )
              return glm::mat4( 1.0f );
            
            const auto time_in_ticks = time_in_seconds * ticks_per_second;
            const auto animation_time = std::fmod( time_in_ticks, duration );
            
            const auto& channel = channels.at( bone_name );
            
            const auto scaling_matrix     = scaling    ( channel, animation_time );
            const auto rotation_matrix    = rotation   ( channel, animation_time );
            const auto translation_matrix = translation( channel, animation_time );
            
            return translation_matrix * rotation_matrix * scaling_matrix;
          }
          
          inline auto scaling( const channel_t& channel, const float animation_time ) const
            -> glm::mat4
          {
            const auto blended_scaling = blending_value( channel.scalings, animation_time );
            return glm::scale( glm::mat4( 1.0f ), blended_scaling );
          }
          
          inline auto rotation( const channel_t& channel, const float animation_time ) const
            -> glm::mat4
          {
            const auto blended_rotation = blending_value( channel.rotations, animation_time );
            return glm::mat4_cast( blended_rotation );
          }
          
          inline auto translation( const channel_t& channel, const float animation_time ) const
            -> glm::mat4
          {
            const auto blended_translation = blending_value( channel.translations, animation_time );
            return glm::translate( glm::mat4( 1.0f ), blended_translation );
          }
          
          template < class T >
          auto blending_value( const T& map, const float animation_time ) const
            -> typename T::mapped_type
          {
            
            auto upper_bound = map.upper_bound( animation_time );
            
            float upper_time, lower_time;
            typename T::mapped_type upper_value, lower_value;
            
            if ( upper_bound not_eq map.end() )
            { 
              upper_value = upper_bound -> second;
              upper_time  = upper_bound -> first;
            }
            else
            {
              upper_value = map.begin() -> second;
              upper_time  = duration;
            }
            
            lower_value = ( --upper_bound ) -> second;
            lower_time  = upper_bound -> first;
            
            //return lower_value;
            
            if ( upper_value == lower_value )
              return upper_value;
            
            const auto blending_factor         = ( animation_time - lower_time ) / ( upper_time - lower_time );
            const auto reverse_blending_factor = 1.0f - blending_factor;
            
            const auto blending_lower_value = lower_value * reverse_blending_factor;
            const auto blending_upper_value = upper_value * blending_factor;
            
            return blending_lower_value + blending_upper_value;
          }
          
        };
        
      }
    }
  }
}