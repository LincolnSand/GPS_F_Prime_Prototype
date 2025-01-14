// ======================================================================
// \title  Gps.hpp
// \author djwait
// \brief  hpp file for Gps component implementation class
// ======================================================================

#ifndef Components_GPSComponent_HPP
#define Components_GPSComponent_HPP

#include "GPSComponent/GPSComponentComponentAc.hpp"

#include "Fw/Types/BasicTypes.hpp"
#include "Fw/Logger/Logger.hpp"
#include <ctype.h>

// Define memory footprint of buffers
// Define a count of buffers & size of each.
// Allow Gps component to manage its own buffers
#define NUM_UART_BUFFERS 20 
#define UART_READ_BUFF_SIZE 1024 

namespace Components {

  class GPSComponent :
    public GPSComponentComponentBase
  {

    public:

      // ----------------------------------------------------------------------
      // Types
      // ----------------------------------------------------------------------

      /*** @brief GpsPacket:
       * * A structured containing the information in the GPS location packet
       * * received via a NEMA GPS receiver
       * * units are as-read from receiver, before parsing*/
      struct GpsPacket {
        float utcTime; // UTC of position hhmmss.ss
        unsigned int date; // Date: ddmmyy

        float dmNS; // Latitude (DDmm.mm)
        char northSouth; // Latitude direction: (N = North, S = South)
        float dmEW; // Longitude (DDDmm.mm)
        char eastWest; // Longitude direction: (E = East, W = West)
        float speedKnots; //Speed over ground, knots
        float speedKmHr; // Speed, kilometres/hour
        float heightWgs84; // Undulation between the geoid and the WGS84 ellipsoid
        float altitude; // Antenna altitude above/below mean sea level
        float dGpsupdate; // Age of correction data (in seconds)
        char dgpsStation[20];
        float trackTrue; // Track made good, degrees True
        float trackMag; // Track made good, degrees Magnetic
        float magVar; // Magnetic variation, degrees
        char magVarDir; // Mag variation direction E/W; (E) subtracts from True course
        char posStatus; // Position status (A = data valid, V = data invalid)
        unsigned int lock; // GPS quality; 0 = Fix not available or invalid
        unsigned int count; // Number of satellites in use
        float HDOP; // Horizontal dilution of precision, 
        float PDOP; // Position dilution of precision
        float VDOP;// Vertical dilution of precision
        char mode; //Positioning system mode indicator
        float filler;
      };

      // ----------------------------------------------------------------------
      // Construction, initialization, and destruction
      // ----------------------------------------------------------------------

      //! Construct object Gps
      //!
      GPSComponent(
          const char *const compName /*!< The component name*/
      );

      //! Initialize object Gps
      //!
      void init(
          const NATIVE_INT_TYPE queueDepth, /*!< The queue depth*/
          const NATIVE_INT_TYPE instance = 0 /*!< The instance number*/
      );

    PRIVATE:

      // ----------------------------------------------------------------------
      // Handler implementations for user-defined typed input ports
      // ----------------------------------------------------------------------

      //! Handler implementation for serialRecv
      //!
      void serialRecv_handler(
          const NATIVE_INT_TYPE portNum, /*!< The port number*/
          Fw::Buffer &serBuffer, /*!< 
      Buffer containing data
      */
          const Drv::RecvStatus& status /*!< 
      Status of read
      */
      ) override;

    PRIVATE:

      // ----------------------------------------------------------------------
      // Command handler implementations
      // ----------------------------------------------------------------------

      //! Implementation for REPORT_STATUS command handler
      //! force an EVR reporting lock status
      void REPORT_STATUS_cmdHandler(
          const FwOpcodeType opCode, /*!< The opcode*/
          const U32 cmdSeq /*!< The command sequence number*/
      );

      //! Implementation for COLD_START command handler
      //! force cold start on reboot
      void COLD_START_cmdHandler(
          const FwOpcodeType opCode, /*!< The opcode*/
          const U32 cmdSeq /*!< The command sequence number*/
      );


      // ----------------------------------------------------------------------
      // Additional member functions & variables
      // ----------------------------------------------------------------------

      //! This will be called once when task starts up
      void preamble() override;

      //!< Has deviced acquired GPS lock?
      bool m_locked;

      //!< Create member variables to store buffers and data array
      // that those buffers use for storage
      Fw::Buffer m_recvBuffers[NUM_UART_BUFFERS];
      BYTE m_uartBuffers[NUM_UART_BUFFERS][UART_READ_BUFF_SIZE];
      char m_holder[UART_READ_BUFF_SIZE];

    };

} // end namespace GpsApp

#endif
