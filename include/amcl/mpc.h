/*
Licensed to the Apache Software Foundation (ASF) under one
or more contributor license agreements.  See the NOTICE file
distributed with this work for additional information
regarding copyright ownership.  The ASF licenses this file
to you under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance
with the License.  You may obtain a copy of the License at

  http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing,
software distributed under the License is distributed on an
"AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
KIND, either express or implied.  See the License for the
specific language governing permissions and limitations
under the License.
*/

/**
 * @file mpc.h
 * @brief MPC declarations
 *
 */

#ifndef MPC_H
#define MPC_H

#include <amcl/amcl.h>
#include <amcl/paillier.h>

#ifdef __cplusplus
extern "C" {
#endif

/** \brief ECDSA Sign message
 *
 *  Generate the ECDSA signature on message, M, with outputs (R,S)
 *
 *  <ol>
 *  <li> Choose a random non-zero value \f$ k \in  F_q \f$ where \f$q\f$ is the curve order
 *  <li> \f$ r_x, r_y = k^{-1}G \f$ where G is the group generator
 *  <li> \f$ r = rx \text{ }\mathrm{mod}\text{ }q \f$
 *  <li> \f$ z = hash(message) \f$
 *  <li> \f$ s = k.(z + r.sk) \text{ }\mathrm{mod}\text{ }q \f$ where \f$ sk \f$ is the ECDSA secret key
 *  </ol>
 *
 *  @param sha is the hash type
 *  @param K Ephemeral key.
 *  @param SK the input private signing key
 *  @param M the input message to be signed
 *  @param R component of the signature
 *  @param S component of the signature
 */
int MPC_ECDSA_SIGN(int sha, octet *K, octet *SK, octet *M, octet *R, octet *S);

/** \brief ECDSA Verify signature
 *
 *  Verify the ECDSA signature (R,S) on a message
 *
 *  @param  HM               Hash of the message
 *  @param  PK               ECDSA public key
 *  @param  R                R component of signature
 *  @param  S                S component of signature
 *  @return                  Returns 0 or else error code
 */
int MPC_ECDSA_VERIFY(octet *HM,octet *PK, octet *R,octet *S);

/** \brief Calculate the inverse of the sum of kgamma values
 *
 *  Calculate the inverse of the sum of kgamma values
 *
 *  <ol>
 *  <li> \f$ invkgamma = (kgamma1 + kgamma2)^{-1} \text{ }\mathrm{mod}\text{ }q \f$
 *  </ol>
 *
 *  @param KGAMMA1            Actor 1 additive share
 *  @param KGAMMA2            Actor 2 additive share
 *  @param INVKGAMMA          Inverse of the sum of the additive shares
 */
void MPC_INVKGAMMA(octet *KGAMMA1, octet *KGAMMA2, octet *INVKGAMMA);

/** \brief R component
 *
 *  Generate the ECDSA signature R component
 *
 *  <ol>
 *  <li> \f$ r_x, r_y = k^{-1}G \f$ where G is the group generator
 *  <li> \f$ r = rx \text{ }\mathrm{mod}\text{ }q \f$
 *  </ol>
 *
 *  @param  INVKGAMMA         Inverse of k times gamma
 *  @param  GAMMAPT1          Actor 1 gamma point
 *  @param  GAMMAPT2          Actor 2 gamma point
 *  @param  R                 R component of the signature
 *  @return                   Returns 0 or else error code
 */
int MPC_R(octet *INVKGAMMA, octet *GAMMAPT1, octet *GAMMAPT2, octet *R);

/** \brief Hash the message value
 *
 *  Hash the message value
 *
 *  @param  sha               Hash type
 *  @param  M                 Message to be hashed
 *  @param  HM                Hash value
 *  @return                   Returns 0 or else error code
 */
void MPC_HASH(int sha, octet *M, octet *HM);

/** \brief S component
 *
 *  Generate the ECDSA signature S component
 *
 *  <ol>
 *  <li> \f$ s = k * (h(m) + sk * r) \text{ }\mathrm{mod}\text{ }q \f$ where h() means hash
 *  <li> \f$ s = (k * h(m)) + (k * sk * r) \text{ }\mathrm{mod}\text{ }q \f$
 *  <li> \f$ s = (k * h(m)) + sigma * r) \text{ }\mathrm{mod}\text{ }q \f$
 *  </ol>
 *
 *  @param  HM                Hash of the message to be signed
 *  @param  R                 R component input
 *  @param  K                 Nonce value
 *  @param  SIGMA             Additive share of k.w
 *  @param  S                 S component output
 *  @return                   Returns 0 or else error code
 */
int MPC_S(octet *HM, octet *R, octet *K, octet *SIGMA, octet *S);

/** \brief Sum of ECDSA s components
 *
 *  Calculate the sum of the s components of the ECDSA signature
 *
 *  <ol>
 *  <li> \f$ s = s1 + s2 \text{ }\mathrm{mod}\text{ }q \f$
 *  </ol>
 *
 *  @param  S1                Actor 1 ECDSA s component
 *  @param  S2                Actor 2 ECDSA s component
 *  @param  S                 S component sum
 */
void MPC_SUM_S(octet *S1, octet *S2, octet *S);

/** \brief Sum of ECDSA public key shares
 *
 *  Calculate the sum of the ECDSA public key shares
 *
 *  <ol>
 *  <li> \f$ pk = pk1 + pk2 \text{ }\mathrm{mod}\text{ }q \f$
 *  </ol>
 *
 *  @param  PK1               Actor 1 ECDSA public key share
 *  @param  PK2               Actor 2 ECDSA public key share
 *  @param  PK                ECDSA public key
 *  @return                   Returns 0 or else error code
 */
int MPC_SUM_PK(octet *PK1, octet *PK2, octet *PK);

/*! \brief Write Paillier public key to octets
 *
 *  @param   PUB              Paillier public key
 *  @param   N                Paillier Modulus - \f$ n = pq \f$
 *  @param   G                Public Base - \f$ g = n+1 \f$
 *  @param   N2               Precomputed \f$ n^2 \f$
 */
void MPC_DUMP_PAILLIER_PK(PAILLIER_public_key *PUB, octet *N, octet *G, octet *N2);

/*! \brief Load Paillier public key from octets
 *
 *  @param   PUB              Paillier public key
 *  @param   N                Paillier Modulus - \f$ n = pq \f$
 *  @param   G                Public Base - \f$ g = n+1 \f$
 *  @param   N2               Precomputed \f$ n^2 \f$
 */
void MPC_LOAD_PAILLIER_PK(PAILLIER_public_key *PUB, octet *N, octet *G, octet *N2);

/*! \brief Write Paillier secret key to octets
 *
 *  @param   PRIV             Paillier secret key
 *  @param   P                Secret prime number
 *  @param   Q                Secret prime number
 *  @param   LP               Private Key modulo \f$ p \f$ (Euler totient of \f$ p \f$)
 *  @param   LQ               Private Key modulo \f$ q \f$ (Euler totient of \f$ q \f$)
 *  @param   INVP             Precomputed \f$ p^{-1} \pmod{2^m} \f$
 *  @param   INVQ             Precomputed \f$ q^{-1} \pmod{2^m} \f$
 *  @param   P2               Precomputed \f$ p^2 \f$
 *  @param   Q2               Precomputed \f$ q^2 \f$
 *  @param   MP               Precomputed \f$ L(g^{lp} \pmod{p^2})^{-1} \f$
 *  @param   MQ               Precomputed \f$ L(g^{lq} \pmod{q^2})^{-1} \f$
 */
void MPC_DUMP_PAILLIER_SK(PAILLIER_private_key *PRIV, octet *P, octet *Q, octet *LP, octet *LQ, octet *INVP, octet *INVQ, octet *P2, octet *Q2, octet *MP, octet *MQ);

/*! \brief Load Paillier secret key from octets
 *
 *  @param   PRIV             Paillier secret key
 *  @param   P                Secret prime number
 *  @param   Q                Secret prime number
 *  @param   LP               Private Key modulo \f$ p \f$ (Euler totient of \f$ p \f$)
 *  @param   LQ               Private Key modulo \f$ q \f$ (Euler totient of \f$ q \f$)
 *  @param   INVP             Precomputed \f$ p^{-1} \pmod{2^m} \f$
 *  @param   INVQ             Precomputed \f$ q^{-1} \pmod{2^m} \f$
 *  @param   P2               Precomputed \f$ p^2 \f$
 *  @param   Q2               Precomputed \f$ q^2 \f$
 *  @param   MP               Precomputed \f$ L(g^{lp} \pmod{p^2})^{-1} \f$
 *  @param   MQ               Precomputed \f$ L(g^{lq} \pmod{q^2})^{-1} \f$
 */
void MPC_LOAD_PAILLIER_SK(PAILLIER_private_key *PRIV, octet *P, octet *Q, octet *LP, octet *LQ, octet *INVP, octet *INVQ, octet *P2, octet *Q2, octet *MP, octet *MQ);

#ifdef __cplusplus
}
#endif

#endif
