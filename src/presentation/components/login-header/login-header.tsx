import React, { memo } from 'react'
import Logo from '../logo/logo'
import Styles from './login-header-styles.scss'

type Props = React.HTMLAttributes<HTMLElement>

const LoginHeader: React.FC<Props> = (props: Props) => {
  return (
    <header className={Styles.header}>
        <Logo />
        <h1>4Dev - Enquetes para Programadores</h1>
    </header>
  )
}

export default memo(LoginHeader)
